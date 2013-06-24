/*
 *  Filename	: EmulationSystem.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef EMULATIONSYSTEM_H_
#define EMULATIONSYSTEM_H_

#define DOT_FILENAME	"/tmp/graph.dot"

extern "C" {
	#include <emu/emu.h>
	#include <emu/emu_cpu_data.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_memory.h>
	#include <emu/emu_shellcode.h>
	#include <emu/emu_graph.h>
	#include <emu/environment/emu_env.h>
	#include <emu/environment/emu_profile.h>
	#include <emu/environment/win32/emu_env_w32.h>
	#include <emu/environment/win32/emu_env_w32_dll.h>
	#include <emu/environment/win32/emu_env_w32_dll_export.h>
	#include <emu/environment/linux/emu_env_linux.h>
	#include <emu/environment/linux/env_linux_syscall_hooks.h>
}

#include <string>

#include <core/ExploitSample.h>

class EmulationUnit;
class Graph;

class EmulationSystem {
public:
	EmulationSystem();
	virtual ~EmulationSystem();

	void loadSample(ExploitSampleHandle sample);
	bool emulate();

private:
	bool drawGraph(GraphHandle graph);

    EmulationUnit *m_emuUnit;
    ExploitSampleHandle m_sample;

	std::string m_graphsDir;
};

#endif /* EMULATIONSYSTEM_H_ */
