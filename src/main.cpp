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
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

/* project headers */
#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
#include <version.h>
#include <toolbox.h>

void parseArguments(int argc, char *argv[]);
void printIntro();
void printUsage();
void printModuleInfo(ModuleInfo *info);
void listInputMods();
void listAnalyzeMods();
void listOutputMods();

/* global options */
string outputMethod = "ConsoleOutput";
list<string> pendingFiles;

int main(int argc, char *argv[]) {
	printIntro();

	/* check cmd arguments */
	parseArguments(argc, argv);
	SHOWVAR(outputMethod);

	/* create main system */
	CoreSystem system;

	int i = 0;
	int exploit_counter = 0;
	SystemError err;
	while(!pendingFiles.empty()) {
		string input_file = pendingFiles.front();
		pendingFiles.pop_front();

		/* check if directory */
		if(isDirectory(input_file)) {
			DIR *dp;
			dirent *de;

			dp = opendir(input_file.c_str());
			if(dp == NULL) {
				PRINTERR("opening directory %s", input_file.c_str());
				continue;
			}

			while((de = readdir(dp))) {
				if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
					continue;

				string next = input_file;
				if(input_file[input_file.size() - 1] != '/')
					next += "/";
				next += de->d_name;
				pendingFiles.push_front(next);
			}

			continue;
		}

		/* load shellcode */
		list<string> files = system.load(input_file);
		if((err = system.getError()) != NO_ERROR) {
			PRINTERR("opening file %s (%s)", input_file.c_str(), system.mapError(err).c_str());
			continue;
		}

		/* process all returned files */
		while(!files.empty()) {
			++i;
			string f = files.front();
			files.pop_front();

			system.emulate(f);
			if((err = system.getError()) != NO_ERROR) {
				PRINTERR("emulating %s (%s)", f.c_str(), system.mapError(err).c_str());
				continue;
			}

			/* analyze graph */
			system.analyze(f);
			if((err = system.getError()) != NO_ERROR) {
				PRINTERR("analyzing %s (%s)", f.c_str(), system.mapError(err).c_str());
				continue;
			}

			if(system.getResults(f)->isShellcodePresent())
				++exploit_counter;

			system.generateOutput(f, outputMethod);
		}
	}
	cout << "FINISHED: found " << exploit_counter << " exploits in " << i << " samples!" << endl;

	return 0;
}

void parseArguments(int argc, char *argv[]) {
	if(argc < 2) {
		SHOWERR("no arguments");
		cout << endl;
		printUsage();
		exit(1);
	}

	for(int i = 1; i < argc; ++i) {
		/* output method */
		if(strcmp(argv[i], "-o") == 0)
			outputMethod = argv[++i];
		/* print analyze modules */
		else if(strcmp(argv[i], "-la") == 0)
			listAnalyzeMods();
		/* print input modules */
		else if(strcmp(argv[i], "-li") == 0)
			listInputMods();
		/* print output modules */
		else if(strcmp(argv[i], "-lo") == 0)
			listOutputMods();
		/* print usage */
		else if(strcmp(argv[i], "-o") == 0) {
			printUsage();
			exit(0);
		}
		/* collect input files */
		else
			pendingFiles.push_back(argv[i]);
	} /* for */

	if(pendingFiles.empty()) {
		SHOWERR("no input files");
		exit(1);
	}
}

void printIntro() {
	cout << "Welcome to graph_analyzer " << VERSION << endl;
	cout << "Author: " << AUTHOR << endl;
	cout << "Build date: " << __DATE__ << endl;
	cout << endl;
}

void printUsage() {
	cout << "GraphAnalyzer " << VERSION << endl;
	cout << "Usage: graph_analyzer [files | folders]" << endl << endl;
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

void listOutputMods() {
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listOutput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "* List of available output modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}
