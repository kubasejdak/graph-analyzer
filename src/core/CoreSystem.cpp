/*
 *  Filename	: CoreSystem.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

CoreSystem::CoreSystem() {
	loadModules();

	SystemLogger::getInstance()->setStatus(IDLE);
	SystemLogger::getInstance()->setError(NO_ERROR);
}

CoreSystem::~CoreSystem() {
	clearCache();
}

list<string> CoreSystem::load(string filename) {
	SystemLogger::getInstance()->setStatus(LOADING);
	SystemLogger::getInstance()->setError(CANNOT_HANDLE_FILE);
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

			/* indicate that input module was found */
			SystemLogger::getInstance()->setError(NO_ERROR);

			break;
		}
	}
	SystemLogger::getInstance()->setStatus(IDLE);

	return files;
}

void CoreSystem::emulate(string filename) {
	SystemLogger::getInstance()->setStatus(EMULATING);
	SystemLogger::getInstance()->setError(NO_ERROR);
	emuSystem.loadSample(samples[filename]);
	bool ret = emuSystem.emulate();
	if(!ret)
		SystemLogger::getInstance()->setError(EMULATION_FAILED);
	SystemLogger::getInstance()->setStatus(IDLE);
}

void CoreSystem::analyze(string filename) {
	SystemLogger::getInstance()->setStatus(ANALYZING);
	SystemLogger::getInstance()->setError(NO_ERROR);
	anaSystem.loadSample(samples[filename]);
	bool ret = anaSystem.analyze();
	if(!ret)
		SystemLogger::getInstance()->setError(ANALYZING_FAILED);
	SystemLogger::getInstance()->setStatus(IDLE);
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
	return SystemLogger::getInstance()->getStatus();
}

SystemError CoreSystem::getError() {
	return SystemLogger::getInstance()->getError();
}

string CoreSystem::mapError(SystemError error) {
	return SystemLogger::getInstance()->mapError(error);
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
