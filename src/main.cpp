/*
 *  Filename	: main.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR		"Kuba Sejdak"

/* standard headers */
#include <iostream>
#include <list>

/* project headers */
#include <core/AnalysisSystem.h>
#include <core/ShellcodeInfo.h>
#include <core/version.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
using namespace std;

void printModuleInfo(ModuleInfo info);
void printShellcodeInfo(ShellcodeInfo info);

int main(int argc, char *argv[]) {
	cout << "Welcome to graph_analyzer " << VERSION << endl;
	cout << "Author: " << AUTHOR << endl;
	cout << "Build date: " << __DATE__ << endl;
	cout << endl;

	if(argc < 2) {
		SHOWERR("no input file");
		return 0;
	}

	AnalysisSystem system;
	string input_file = argv[1];

	list<ModuleInfo> mods = ModuleManager::getInstance()->listInput();
	list<ModuleInfo>::iterator it;
	cout << "List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it) {
		cout << "============================================" << endl;
		printModuleInfo((*it));
	}
	cout << endl;

	bool ret;
	ret = system.loadShellcode(input_file);
	if(!ret) {
		SHOWERR("opening file");
		return 0;
	}

	ret = system.analyze(input_file);
	if(ret) {
		ShellcodeInfo info = system.getResults(input_file);
		cout << "Results for " << input_file << ":" << endl;
		info.printInfo();
	}
	else
		SHOWERR("analyzing file");

	return 0;
}

void printModuleInfo(ModuleInfo info) {
	cout << "name: " << info.name << endl;
	cout << "id: " << info.id << endl;
	cout << "description: " << info.description << endl;
}
