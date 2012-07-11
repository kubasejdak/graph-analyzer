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
	listInputMods();
	listAnalyzeMods();

	bool ret;

	for(int i = 1; i < argc; ++i) {
		string input_file = argv[i];

		/* load shellcode */
		ret = system.loadShellcode(input_file);
		if(!ret) {
			PRINTERR("opening file %s", input_file.c_str());
			continue;
		}

		/* emulate shellcode */
		ret = system.emulate(input_file);
		if(!ret) {
			PRINTERR("emulating %s", input_file.c_str());
			continue;
		}

		/* analyze graph */
		ret = system.analyze(input_file);
		if(!ret) {
			PRINTERR("analyzing %s", input_file.c_str());
			continue;
		}

		ShellcodeInfo *info = system.getResults(input_file);
		cout << "Results for sample #" << i << " :" << endl;
		info->printInfo();
		cout << endl;
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
	cout << "name: " << info->name << endl;
	cout << "id: " << info->id << endl;
	cout << "description: " << info->description << endl;
}

void listInputMods() {
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listInput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it) {
		cout << "============================================" << endl;
		printModuleInfo((*it));
	}
	cout << endl;
}

void listAnalyzeMods() {
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listAnalyze();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "List of available analyze modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it) {
		cout << "============================================" << endl;
		printModuleInfo((*it));
	}
	cout << endl;
}
