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

list<string> CoreSystem::load(string filename) {
	list<string> files;
	queue<ShellcodeSample *> q;
	ShellcodeSample *s;
	FileAnalyser fileAnalyser;
	string fileType = fileAnalyser.analyze(filename);

	map<int, AbstractInput *>::iterator it;
	for(it = inputModules->begin(); it!= inputModules->end(); ++it) {
		if(fileType == (*it).second->getType()) {
			(*it).second->loadInput(filename, &q);

			/* process all returned files */
			while(!q.empty()) {
				s = q.front();
				q.pop();
				samples[s->getInfo()->getName()] = s;
				files.push_back(s->getInfo()->getName());
			}

			break;
		}
	}

	return files;
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

ShellcodeInfo *CoreSystem::getResults(string filename) {
	if(samples.find(filename) == samples.end())
		return new ShellcodeInfo();

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
