/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

/* standard headers */
#include <iostream>
#include <list>

/* project headers */
#include <core/AnalysisSystem.h>
#include <core/version.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
using namespace std;

int main(int argc, char *argv[]) {
	cout << "Welcome to graph analyzer " << VERSION << endl;
	cout << endl;

	AnalysisSystem system;
	system.loadShellcode("dupa.txt");

	list<ModuleInfo> mods = ModuleManager::getInstance()->listInput();
	list<ModuleInfo>::iterator it;
	cout << "List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it) {
		cout << "name: " << (*it).name << endl;
		cout << "id: " << (*it).id << endl;
		cout << "description: " << (*it).description << endl;
	}

	return 0;
}
