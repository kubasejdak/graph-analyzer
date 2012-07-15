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
	InstructionSplitter splitter;
	for(it = g->begin(); it != g->end(); ++it) {
		instr_vert = (struct instr_vertex *) it->data;
		if(instr_vert->dll) {
			splitter = emu_string_char(instr_vert->instr_string);
			dll = instr_vert->dll->dllname;
			dll += ".dll";
			syscall = splitter.getSyscall();
			sample->getInfo()->setTrait("API", syscall);
			sample->getInfo()->setTrait("DLL", dll);
		}
	}

	return true;
}
