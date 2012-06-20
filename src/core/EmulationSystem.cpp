/*
 *  Filename	: EmulationSystem.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#include "EmulationSystem.h"

EmulationSystem::EmulationSystem() {
	emuUnit = new EmulationUnit();
	sample = NULL;
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

	/* start emulation */
	for(int i = 0; i < EMULATION_STEPS; ++i) {
		// TODO: implement
	}

	sample = NULL;
	return true;
}
