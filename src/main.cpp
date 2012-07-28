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

	/* collect input files */
	list<string> pendingFiles;
	for(int i = 1; i < argc; ++i)
		pendingFiles.push_back(argv[i]);

	/* create main system */
	CoreSystem system;

	/* list available modules */
	cout << "============================================" << endl;
	listInputMods();
	listAnalyzeMods();
	cout << "============================================" << endl;

	bool ret;
	int i = 1;
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
			cout << "Results for sample #" << dec << i << " :" << endl;
			info->printInfo();
			cout << endl;
			++i;
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
