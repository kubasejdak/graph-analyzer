/*
 *  Filename	: SyscallAnalyze.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#include "SyscallAnalyze.h"

SyscallAnalyze::SyscallAnalyze()
{
    m_name = "SyscallAnalyze";
    m_description = "Searches the graph for system API calls.";
    m_traitName = "api";
}

bool SyscallAnalyze::perform(ShellcodeSample *sample)
{
    LOG("\n");
    Graph *g = sample->graph();
	struct instr_vertex *instr_vert;
    QString syscall, dll;
	Graph::graph_iterator it;
	InstructionSplitter splitter;
    QMap<QString, QString> *m;

	for(it = g->begin(); it != g->end(); ++it) {
		instr_vert = (struct instr_vertex *) it->data;
		if(instr_vert->dll) {
			splitter = emu_string_char(instr_vert->instr_string);
			dll = instr_vert->dll->dllname;
			dll += ".dll";
            syscall = splitter.syscall();
            m = new QMap<QString, QString> ();
			(*m)["syscall"] = syscall;
            LOG("syscall: [%s]\n", syscall.toStdString().c_str());
			(*m)["DLL"] = dll;
            LOG("DLL: [%s]\n", dll.toStdString().c_str());
            sample->info()->setTrait(m_traitName, m);
		}
	}

    LOG("SUCCESS\n");
	return true;
}
