/*
 *  Filename	: main.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

/* standard headers */
#include <iostream>
#include <list>
using namespace std;

/* project headers */
#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
#include <version.h>
#include <toolbox.h>

void printIntro();
void printModuleInfo(ModuleInfo *info);
void listInputMods();

int main(int argc, char *argv[]) {
	printIntro();

	/* check cmd arguments */
	if(argc < 2) {
		SHOWERR("no input file");
		return 1;
	}
	string input_file = argv[1];

	/* create main system */
	CoreSystem system;

	/* list available modules */
	listInputMods();

	bool ret;

	/* load shellcode */
	ret = system.loadShellcode(input_file);
	if(!ret) {
		SHOWERR("opening file");
		return 1;
	}

	/* emulate shellcode */
	ret = system.emulate(input_file);
	if(!ret) {
		SHOWERR("emulating");
		return 1;
	}

	/* analyze graph */
	ret = system.analyze(input_file);
	if(!ret) {
		SHOWERR("analyzing");
		return 1;
	}

	ShellcodeInfo info = system.getResults(input_file);
	cout << "Results for " << input_file << ":" << endl;
	info.printInfo();

	return 0;
}

void printIntro() {
	cout << "Welcome to graph_analyzer " << VERSION << endl;
	cout << "Author: " << AUTHOR << endl;
	cout << "Build date: " << __DATE__ << endl;
	cout << endl;
}

void printModuleInfo(ModuleInfo *info) {
	cout << "name: " << info->name << endl;
	cout << "id: " << info->id << endl;
	cout << "description: " << info->description << endl;
}

void listInputMods() {
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listInput();
	list<ModuleInfo *>::iterator it;
	cout << "List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it) {
		cout << "============================================" << endl;
		printModuleInfo((*it));
	}
	cout << endl;
}
