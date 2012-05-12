/*
 *  Filename	: EmulationSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#include "EmulationSystem.h"

EmulationSystem::EmulationSystem() {
	emuUnit = new EmulationUnit();
	sample = NULL;

	loadModules();
}

EmulationSystem::~EmulationSystem() {
	delete emuUnit;
}

void EmulationSystem::loadSample(ShellcodeSample *sample) {
	this->sample = sample;
}

bool EmulationSystem::emulate() {
	if(!sample)
		return false;

	// emulate here

	sample = NULL;
	return true;
}

void EmulationSystem::loadModules() {
	analyzeModules = ModuleManager::getInstance()->getAnalyze();
}
