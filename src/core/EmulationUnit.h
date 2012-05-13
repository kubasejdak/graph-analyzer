/*
 *  Filename	: EmulationUnit.h
 *  Author		: Kuba Sejdak
 *  Created on	: 12-05-2012
 */

#ifndef EMULATIONUNIT_H_
#define EMULATIONUNIT_H_

#define STATIC_CODE_OFFSET	0x417000

/* libemu headers */
extern "C" {
	#include <emu/emu.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_memory.h>
	#include <emu/emu_shellcode.h>
	#include <emu/environment/emu_env.h>
	#include <emu/environment/emu_profile.h>
}

/* standard headers */
#include <stdint.h>

/* project headers */
#include <core/toolbox.h>
#if 0
#include <core/UserHooks.h>
#endif

class EmulationUnit {
public:
	EmulationUnit();
	virtual ~EmulationUnit();

	void prepareUnit();
	void destroyUnit();
	void resetUnit();

	bool step();
	int32_t loadCode(byte_t *code, int size);

	struct emu *getEmu();
	struct emu_cpu *getCpu();
	struct emu_memory *getMemory();
	struct emu_env *getEnv();
	int32_t getCodeOffset();

private:
	int32_t getPcTest(byte_t *code, int size);
	void prepareIATForSQLSlammer();
	void exportWin32Hooks();
	void exportLinuxHooks();

	struct emu *emu;
	struct emu_cpu *cpu;
	struct emu_memory *mem;
	struct emu_env *env;

	int32_t codeOffset;
};

#endif /* EMULATIONUNIT_H_ */
