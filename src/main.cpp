/*
 *  Filename	: main.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

/* standard headers */
#include <iostream>
#include <list>
#include <iomanip>
using namespace std;

/* project headers */
#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
#include <version.h>

void printIntro();
void printUsage();
void printModuleInfo(ModuleInfo *info);
void listInputMods();
void listAnalyzeMods();

int main(int argc, char *argv[]) {
	printIntro();

	/* check cmd arguments */
	if(argc < 2) {
		SHOWERR("no input files");
		cout << endl;
		printUsage();
		return 1;
	}

	/* create main system */
	CoreSystem system;

	/* list available modules */
	cout << "============================================" << endl;
	listInputMods();
	listAnalyzeMods();
	cout << "============================================" << endl;

	bool ret;
	for(int i = 1; i < argc; ++i) {
		string input_file = argv[i];

		/* load shellcode */
		list<string> files = system.load(input_file);
		if(files.empty()) {
			PRINTERR("opening file %s", input_file.c_str());
			continue;
		}

		/* process all returned files */
		while(!files.empty()) {
			string f = files.front();
			files.pop_front();

			ret = system.emulate(f);
			if(!ret) {
				PRINTERR("emulating %s", f.c_str());
				continue;
			}

			/* analyze graph */
			ret = system.analyze(f);
			if(!ret) {
				PRINTERR("analyzing %s", f.c_str());
				continue;
			}

			ShellcodeInfo *info = system.getResults(f);
			cout << "Results for sample #" << i << " :" << endl;
			info->printInfo();
			cout << endl;
		}
	}

	return 0;
}

void printIntro() {
	cout << "Welcome to graph_analyzer " << VERSION << endl;
	cout << "Author: " << AUTHOR << endl;
	cout << "Build date: " << __DATE__ << endl;
	cout << endl;
}

void printUsage() {
	cout << "GraphAnalyzer " << VERSION << endl;
	cout << "Usage: graph_analyzer [files]" << endl << endl;
	cout << "Report bugs to kuba.sejdak@gmail.com" << endl;
}

void printModuleInfo(ModuleInfo *info) {
	cout << "id: " << setw(6) << left << info->id << " ";
	cout << "name: " << setw(17) << left << info->name << " ";
	cout << "description: " << info->description << endl;
}

void listInputMods() {
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listInput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;

	cout << "* List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}

void listAnalyzeMods() {
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listAnalyze();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "* List of available analyze modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}
