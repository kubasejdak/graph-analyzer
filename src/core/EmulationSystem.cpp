/*
 *  Filename	: EmulationSystem.cpp
 *  Author	: Kuba Sejdak
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

	/* load code to emu unit */
	int32_t codeOffset;
	codeOffset = emuUnit->loadCode(sample->getCode(), sample->getInfo().getSize());
	sample->getInfo().setCodeOffset(codeOffset);
	sample->getInfo().setShellcodePresent(codeOffset >= 0 ? true : false);

	/* start emulating CPU steps */
	bool status;
	map<int, AbstractAnalyze *>::iterator it;
	for(int i = 0; i < EMULATION_STEPS; ++i) {
		for(it = analyzeModules->begin(); it != analyzeModules->end(); ++it) {
			status = (*it).second->perform(emuUnit);
			if(!status)
				return false;
		}

		status = emuUnit->step();
		if(!status)
			break;
	}

	return true;
}

void EmulationSystem::loadModules() {
	analyzeModules = ModuleManager::getInstance()->getAnalyze();
}
