/*
 *  Filename	: CoreSystem.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

CoreSystem::CoreSystem() {
	loadModules();

	changeStatus(IDLE);
}

CoreSystem::~CoreSystem() {
	clearCache();
}

bool CoreSystem::loadShellcode(string filename) {
	ShellcodeSample *s = new ShellcodeSample();
	if(!s)
		return false;

	map<int, AbstractInput *>::iterator it;
	for(it = inputModules->begin(); it!= inputModules->end(); ++it) {
		if(iends_with(filename, (*it).second->getExtension())) {
			bool ret = (*it).second->loadInput(filename, s);
			if(ret)
				samples[filename] = s;

			return ret;
		}
	}

	return false;
}

bool CoreSystem::emulate(string filename) {
	changeStatus(EMULATING);
	emuSystem.loadSample(samples[filename]);
	bool ret = emuSystem.emulate();
	changeStatus(IDLE);

	return ret;
}

bool CoreSystem::analyze(string filename) {
	changeStatus(ANALYZING);
	anaSystem.loadSample(samples[filename]);
	bool ret = anaSystem.analyze();
	changeStatus(IDLE);

	return ret;
}

ShellcodeInfo CoreSystem::getResults(string filename) {
	if(samples.find(filename) == samples.end())
		return ShellcodeInfo();

	return samples[filename]->getInfo();
}

bool CoreSystem::generateOutput(string filename, int method, string *output) {
	/* TODO: implement */
	return true;
}

SystemStatus CoreSystem::getStatus() {
	return status;
}

void CoreSystem::clearCache() {
	map<string, ShellcodeSample *>::iterator it;
	for(it = samples.begin(); it != samples.end(); ++it) {
		delete (*it).second;
		samples.erase(it);
	}
}

void CoreSystem::loadModules() {
	inputModules = ModuleManager::getInstance()->getInput();
	outputModules = ModuleManager::getInstance()->getOutput();
}

void CoreSystem::changeStatus(SystemStatus status) {
	this->status = status;
}
