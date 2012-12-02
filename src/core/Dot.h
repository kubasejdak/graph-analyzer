/*
 * Filename		: Dot.h
 * Author		: Kuba Sejdak
 * Created on	: 21-06-2012
 */

#ifndef DOT_H_
#define DOT_H_

#include <QString>

extern "C" {
	#include <emu/emu.h>
	#include <emu/emu_memory.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_cpu_data.h>
	#include <emu/emu_cpu_stack.h>
	#include <emu/emu_string.h>
	#include <emu/emu_graph.h>
	#include <emu/environment/emu_profile.h>
	#include <emu/environment/emu_env.h>
	#include <emu/environment/linux/emu_env_linux.h>
	#include <emu/environment/win32/emu_env_w32.h>
	#include <emu/environment/win32/emu_env_w32_dll.h>
	#include <emu/environment/win32/emu_env_w32_dll_export.h>
	#include <emu/environment/win32/env_w32_dll_export_kernel32_hooks.h>
	#include <emu/environment/linux/env_linux_syscall_hooks.h>
}

#include <core/SystemLogger.h>
//#include <core/Options.h>

struct instr_vertex {
	uint32_t eip;
	struct emu_string  *instr_string;
	struct emu_env_w32_dll *dll;
	struct emu_env_linux_syscall *syscall;
};


struct instr_vertex *instr_vertex_new(uint32_t theeip, const char *instr_string);
void instr_vertex_free(struct instr_vertex *iv);

struct instr_vertex *instr_vertex_copy(struct instr_vertex *from);

void instr_vertex_destructor(void *data);

void graph_draw(struct emu_graph *graph, QString dot_file);

#endif /* DOT_H_ */
