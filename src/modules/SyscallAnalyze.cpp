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
	Graph *g = sample->getGraph();
	struct instr_vertex *instr_vert;
	string syscall, dll;
	Graph::graph_iterator it;
	for(it = g->begin(); it != g->end(); ++it) {
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
