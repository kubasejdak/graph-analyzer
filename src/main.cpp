/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

/* standard headers */
#include <boost/program_options.hpp>
#include <iostream>
#include <list>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;
namespace opt = boost::program_options;

/* project headers */
#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
#include <toolbox.h>

void printIntro(string version);
void printModuleInfo(ModuleInfo *info);
void listInputMods();
void listAnalyzeMods();
void listOutputMods();

int main(int argc, char *argv[])
{
	/* create main system */
	CoreSystem system;
	printIntro(system.getVersion());

	/* parse cmd line arguments */
	vector<string> input, output;
	string log_file, log_level;
	opt::options_description desc("Options");
	desc.add_options()
		("help,h", "print help message")
		("input,i", opt::value<vector<string> >(&input), "set input files")
		("output,o", opt::value<vector<string> >(&output), "set output destinations (default: ConsoleOutput)")
		("list-analyze", "list analyze modules")
		("list-input", "list input modules")
		("list-output", "list output modules")
		("log-file", opt::value<string>(&log_file), "set file to save logs")
		("log-level", opt::value<string>(&log_level), "set logging level")
	;

	opt::positional_options_description p;
	p.add("input", -1);

	opt::variables_map vm;
	opt::store(opt::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	opt::notify(vm);

	/* check any arguments */
	if(vm.empty()) {
		SHOWERR("no arguments");
		exit(1);
	}
	/* help */
	if(vm.count("help")) {
		cout << desc << endl;
		exit(0);
	}
	/* list-analyze */
	if(vm.count("list-analyze")) {
		listAnalyzeMods();
		exit(0);
	}
	/* list-input */
	if(vm.count("list-input")) {
		listInputMods();
		exit(0);
	}
	/* list-output */
	if(vm.count("list-output")) {
		listOutputMods();
		exit(0);
	}
	/* log-file */
	if(vm.count("log-file"))
		system.setLogFile(log_file);
	/* log-level */
	if(vm.count("log-level"))
		system.setLogLevel(atoi(log_level.c_str()));
	/* input */
	if(!vm.count("input")) {
		SHOWERR("no input files");
		exit(1);
	}
	while(!input.empty()) {
		system.addFile(input.back());
		input.pop_back();
	}
	/* output */
	if(!vm.count("output")) {
		SHOWMSG("no output destination, setting to ConsoleOutput");
		system.setOutput();
	}
	while(!output.empty()) {
		system.setOutput(output.back());
		output.pop_back();
	}

	/* run */
	system.run();

	int s_num = system.getSamplesNum();
	int e_num = system.getExploitsNum();
	string e = (e_num == 1) ? "exploit" : "exploits";
	string s = (s_num == 1) ? "sample" : "samples";
	cout << "FINISHED: found " << dec << e_num << " " << e << " in " << s_num
			<< " " << s << "!" << endl;

	return 0;
}

void printIntro(string version)
{
	cout << "graph-analyzer " << version << endl;
	cout << "Author: " << AUTHOR << endl;
	cout << "Build date: " << __DATE__ << endl;
	cout << endl;
}

void printModuleInfo(ModuleInfo *info)
{
	cout << "id: " << setw(6) << left << info->id << " ";
	cout << "name: " << setw(17) << left << info->name << " ";
	cout << "description: " << info->description << endl;
}

void listInputMods()
{
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listInput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;

	cout << "* List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}

void listAnalyzeMods()
{
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listAnalyze();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "* List of available analyze modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}

void listOutputMods()
{
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listOutput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "* List of available output modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}
