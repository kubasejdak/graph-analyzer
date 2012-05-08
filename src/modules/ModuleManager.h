/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

/* standard headers */
#include <map>
#include <list>

/* project headers */
#include <modules/AbstractInput.h>
#include <modules/AbstractOutput.h>
#include <modules/AbstractAnalyze.h>
#include <modules/ModuleInfo.h>
#include <modules/BinaryInput.h>

class ModuleManager {
public:
	virtual ~ModuleManager();
	static ModuleManager * getInstance() {
		static ModuleManager instance;
	    return &instance;
	}

	map<int, AbstractInput *> * getInput();
	map<int, AbstractOutput *> * getOutput();
	map<int, AbstractAnalyze *> * getAnalyze();

	list<ModuleInfo> listInput();
	list<ModuleInfo> listOutput();
	list<ModuleInfo> listAnalyze();

private:
	ModuleManager();

	void loadInput();
	void loadOutput();
	void loadAnalyze();

	void removeInput();
	void removeOutput();
	void removeAnalyze();

	map<int, AbstractInput *> inputModules;
	map<int, AbstractOutput *> outputModules;
	map<int, AbstractAnalyze *> analyzeModules;
};

#endif /* MODULEMANAGER_H_ */
