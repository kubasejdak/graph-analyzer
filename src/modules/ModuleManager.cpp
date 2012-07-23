/*
 *  Filename	: ModuleManager.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "ModuleManager.h"

ModuleManager::ModuleManager() {
	loadInput();
	loadOutput();
	loadAnalyze();
}

ModuleManager::~ModuleManager() {
	removeInput();
	removeOutput();
	removeAnalyze();
}

void ModuleManager::loadInput() {
	/* BinaryInput */
	BinaryInput *binaryInput = new BinaryInput();
	inputModules[binaryInput->getId()] = binaryInput;
}

void ModuleManager::loadOutput() {
}

void ModuleManager::loadAnalyze() {
	/* SyscallAnalyze */
	SyscallAnalyze *syscallAnalyze = new SyscallAnalyze();
	analyzeModules[syscallAnalyze->getId()] = syscallAnalyze;

	/* GraphHash */
	GraphHash *graphHash = new GraphHash();
	analyzeModules[graphHash->getId()] = graphHash;

	/* LoopDetector */
	LoopDetector *loopDetector = new LoopDetector();
	analyzeModules[loopDetector->getId()] = loopDetector;
}

map<int, AbstractInput *> * ModuleManager::getInput() {
	return &inputModules;
}

map<int, AbstractOutput *> * ModuleManager::getOutput() {
	return &outputModules;
}

map<int, AbstractAnalyze *> * ModuleManager::getAnalyze() {
	return &analyzeModules;
}

list<ModuleInfo *> ModuleManager::listInput() {
	list<ModuleInfo *> modList;
	map<int, AbstractInput *>::iterator it;
	for(it = inputModules.begin(); it != inputModules.end(); ++it)
		modList.push_back((*it).second->getModuleInfo());

	return modList;
}

list<ModuleInfo *> ModuleManager::listOutput() {
	list<ModuleInfo *> modList;
	map<int, AbstractOutput *>::iterator it;
	for(it = outputModules.begin(); it != outputModules.end(); ++it)
		modList.push_back((*it).second->getModuleInfo());

	return modList;
}

list<ModuleInfo *> ModuleManager::listAnalyze() {
	list<ModuleInfo *> modList;
	map<int, AbstractAnalyze *>::iterator it;
	for(it = analyzeModules.begin(); it != analyzeModules.end(); ++it)
		modList.push_back((*it).second->getModuleInfo());

	return modList;
}

void ModuleManager::removeInput() {
	map<int, AbstractInput *>::iterator it;
	for(it = inputModules.begin(); it != inputModules.end(); ++it)
		inputModules.erase(it);
}

void ModuleManager::removeOutput() {
	map<int, AbstractOutput *>::iterator it;
	for(it = outputModules.begin(); it != outputModules.end(); ++it)
		outputModules.erase(it);
}

void ModuleManager::removeAnalyze() {
	map<int, AbstractAnalyze *>::iterator it;
	for(it = analyzeModules.begin(); it != analyzeModules.end(); ++it)
		analyzeModules.erase(it);
}
