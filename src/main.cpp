/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
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

	bool success = system.loadShellcode("test_file.bin");
	if(success) {
		ShellcodeInfo info = system.getResults("test_file.bin");
		cout << "Results for test_file.bin:" << endl;
		printShellcodeInfo(info);
	}
	else
		SHOWERR("opening test_file.bin");

	return 0;
}

void printModuleInfo(ModuleInfo info) {
	cout << "name: " << info.name << endl;
	cout << "id: " << info.id << endl;
	cout << "description: " << info.description << endl;
}
void printShellcodeInfo(ShellcodeInfo info) {
	cout << "name: " << info.name << endl;
	cout << "size: " << info.size << endl;
	cout << "code offset: " << info.codeOffset << endl;
	cout << "shellcode present: " << (info.shellcodePresent ? "yes" : "no") << endl;
	cout << "general type: " << info.genType << endl;
	cout << "connection type: " << info.connType << endl;
	cout << "used system API: ";
	list<string>::iterator it;
	for(it = info.usedSysAPI.begin(); it != info.usedSysAPI.end(); ++it)
		cout << (*it) << "() ";
	cout << endl;
}
