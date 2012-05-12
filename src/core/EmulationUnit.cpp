/*
 *  Filename	: EmulationUnit.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 12-05-2012
 */

#include "EmulationUnit.h"

EmulationUnit::EmulationUnit() {
	emu = emu_new();
	env = emu_env_new(emu);
}

EmulationUnit::~EmulationUnit() {
	emu_env_free(env);
	emu_free(emu);
}

struct emu *EmulationUnit::getEmu() {
	return emu;
}

struct emu_env *EmulationUnit::getEnv() {
	return env;
}

void EmulationUnit::exportWin32Hooks() {
}

void EmulationUnit::exportLinuxHooks() {
}
