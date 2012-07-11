/*
 *  Filename	: SyscallAnalyze.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#include "SyscallAnalyze.h"

SyscallAnalyze::SyscallAnalyze() {
	id = getNextID();
	name = "SyscallAnalyze";
	description = "Searches the graph for system API calls.";
}

SyscallAnalyze::~SyscallAnalyze() {
}

bool SyscallAnalyze::perform(ShellcodeSample *sample) {
	struct emu_vertex_root *root = sample->getGraph()->getEmuGraph()->vertexes;
	struct emu_vertex *it = emu_vertexes_first(root);
	struct instr_vertex *instr_vert;
	string syscall;
	string dll;
	for(; !emu_vertexes_attail(it); it = emu_vertexes_next(it)) {
		instr_vert = (struct instr_vertex *) it->data;
		if(instr_vert->dll) {
			syscall = emu_string_char(instr_vert->instr_string);
			dll = instr_vert->dll->dllname;
			syscall = cutSyscallName(syscall, dll);
			sample->getInfo()->setTrait("API", syscall);
		}
	}

	return true;
}

string SyscallAnalyze::cutSyscallName(string instr, string dll) {
	instr.erase(instr.size() - 2, 2);
	int n = instr.find_last_of(" ");
	instr.erase(0, n + 1);
	instr += " (";
	instr += dll;
	instr += ".dll)";

	return instr;
}
