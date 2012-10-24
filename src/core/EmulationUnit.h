/*
 *  Filename	: EmulationUnit.h
 *  Author		: Kuba Sejdak
 *  Created on	: 12-05-2012
 */

#ifndef EMULATIONUNIT_H_
#define EMULATIONUNIT_H_

#define STATIC_CODE_OFFSET	0x417000

extern "C" {
	#include <emu/emu.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_memory.h>
	#include <emu/emu_shellcode.h>
	#include <emu/environment/emu_env.h>
	#include <emu/environment/emu_profile.h>
}

#include <stdint.h>

#include <core/Toolbox.h>
#include <core/SystemLogger.h>
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
	int32_t loadCode(byte_t *code, int32_t size);

    struct emu *emu();
    struct emu_cpu *cpu();
    struct emu_memory *memory();
    struct emu_env *env();
    int32_t codeOffset();

private:
	int32_t getPcTest(byte_t *code, int32_t size);
	void prepareIATForSQLSlammer();
	void exportWin32Hooks();
	void exportLinuxHooks();

    struct emu *m_emu;
    struct emu_cpu *m_cpu;
    struct emu_memory *m_mem;
    struct emu_env *m_env;

    int32_t m_codeOffset;
};

#endif /* EMULATIONUNIT_H_ */
