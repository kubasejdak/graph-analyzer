/*
 *  Filename	: main.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR		"Kuba Sejdak"
#define TEST_FILE	"shell_1.bin"

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

	AnalysisSystem system;

	list<ModuleInfo> mods = ModuleManager::getInstance()->listInput();
	list<ModuleInfo>::iterator it;
	cout << "List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it) {
		cout << "============================================" << endl;
		printModuleInfo((*it));
	}
	cout << endl;

	bool ret;
	ret = system.loadShellcode(TEST_FILE);
	if(!ret) {
		SHOWERR("opening file");
		return 0;
	}

	ret = system.analyze(TEST_FILE);
	if(ret) {
		ShellcodeInfo info = system.getResults(TEST_FILE);
		cout << "Results for " << TEST_FILE << ":" << endl;
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
