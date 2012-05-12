/*
 *  Filename	: EmulationUnit.h
 *  Author		: Kuba Sejdak
 *  Created on	: 12-05-2012
 */

#ifndef EMULATIONUNIT_H_
#define EMULATIONUNIT_H_

/* libemu headers */
extern "C" {
	#include <emu/emu.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_memory.h>
	#include <emu/environment/emu_env.h>
}

class EmulationUnit {
public:
	EmulationUnit();
	virtual ~EmulationUnit();

	struct emu *getEmu();
	struct emu_env *getEnv();

private:
	void exportWin32Hooks();
	void exportLinuxHooks();

	struct emu *emu;
	struct emu_env *env;
};

#endif /* EMULATIONUNIT_H_ */
