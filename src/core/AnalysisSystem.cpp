/*
 *  Filename	: AnalysisSystem.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "AnalysisSystem.h"

AnalysisSystem::AnalysisSystem() {
	loadModules();

	changeStatus(IDLE);
}

AnalysisSystem::~AnalysisSystem() {
	clearCache();
}

bool AnalysisSystem::loadShellcode(string filename) {
	ShellcodeSample *s = new ShellcodeSample();
	if(!s)
		return false;

	map<int, AbstractInput *>::iterator it;
	for(it = inputModules->begin(); it!= inputModules->end(); ++it) {
		if(iends_with(filename, (*it).second->getExtension())) {
			bool ret = (*it).second->loadInput(filename, s);
			if(ret)
				samples.insert(pair<string, ShellcodeSample *>(filename, s));

			return ret;
		}
	}

	return false;
}

bool AnalysisSystem::analyze(string filename) {
	changeStatus(WORKING);
	emuSystem.loadSample(samples[filename]);
	bool ret = emuSystem.emulate();
	changeStatus(IDLE);

	return ret;
}

ShellcodeInfo AnalysisSystem::getResults(string filename) {
	if(samples.find(filename) == samples.end())
		return ShellcodeInfo();

	return samples[filename]->getInfo();
}

bool AnalysisSystem::generateOutput(string filename, int method, string *output) {
	/* TODO: implement */
	return true;
}

SystemStatus AnalysisSystem::getStatus() {
	return status;
}

void AnalysisSystem::clearCache() {
	map<string, ShellcodeSample *>::iterator it;
	for(it = samples.begin(); it != samples.end(); ++it) {
		delete (*it).second;
		samples.erase(it);
	}
}

void AnalysisSystem::loadModules() {
	inputModules = ModuleManager::getInstance()->getInput();
	outputModules = ModuleManager::getInstance()->getOutput();
}

void AnalysisSystem::changeStatus(SystemStatus status) {
	this->status = status;
}
