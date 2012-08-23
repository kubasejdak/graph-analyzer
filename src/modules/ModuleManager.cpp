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
	inputModules[binaryInput->getName()] = binaryInput;

	/* BinaryInput */
	PcapInput *pcapInput = new PcapInput();
	inputModules[pcapInput->getName()] = pcapInput;
}

void ModuleManager::loadOutput() {
	/* DatabaseOutput */
	DatabaseOutput *databaseOutput = new DatabaseOutput();
	outputModules[databaseOutput->getName()] = databaseOutput;

	/* DatabaseOutput */
	ConsoleOutput *consoleOutput = new ConsoleOutput();
	outputModules[consoleOutput->getName()] = consoleOutput;
}

void ModuleManager::loadAnalyze() {
	/* SyscallAnalyze */
	SyscallAnalyze *syscallAnalyze = new SyscallAnalyze();
	analyzeModules[syscallAnalyze->getName()] = syscallAnalyze;

	/* GraphHash */
	GraphHash *graphHash = new GraphHash();
	analyzeModules[graphHash->getName()] = graphHash;

	/* LoopDetector */
	LoopDetector *loopDetector = new LoopDetector();
	analyzeModules[loopDetector->getName()] = loopDetector;
}

map<string, AbstractInput *> * ModuleManager::getInput() {
	return &inputModules;
}

map<string, AbstractOutput *> * ModuleManager::getOutput() {
	return &outputModules;
}

map<string, AbstractAnalyze *> * ModuleManager::getAnalyze() {
	return &analyzeModules;
}

list<ModuleInfo *> ModuleManager::listInput() {
	list<ModuleInfo *> modList;
	map<string, AbstractInput *>::iterator it;
	for(it = inputModules.begin(); it != inputModules.end(); ++it)
		modList.push_back((*it).second->getModuleInfo());

	return modList;
}

list<ModuleInfo *> ModuleManager::listOutput() {
	list<ModuleInfo *> modList;
	map<string, AbstractOutput *>::iterator it;
	for(it = outputModules.begin(); it != outputModules.end(); ++it)
		modList.push_back((*it).second->getModuleInfo());

	return modList;
}

list<ModuleInfo *> ModuleManager::listAnalyze() {
	list<ModuleInfo *> modList;
	map<string, AbstractAnalyze *>::iterator it;
	for(it = analyzeModules.begin(); it != analyzeModules.end(); ++it)
		modList.push_back((*it).second->getModuleInfo());

	return modList;
}

void ModuleManager::removeInput() {
	map<string, AbstractInput *>::iterator it;
	for(it = inputModules.begin(); it != inputModules.end(); ++it)
		inputModules.erase(it);
}

void ModuleManager::removeOutput() {
	map<string, AbstractOutput *>::iterator it;
	for(it = outputModules.begin(); it != outputModules.end(); ++it)
		outputModules.erase(it);
}

void ModuleManager::removeAnalyze() {
	map<string, AbstractAnalyze *>::iterator it;
	for(it = analyzeModules.begin(); it != analyzeModules.end(); ++it)
		analyzeModules.erase(it);
}
