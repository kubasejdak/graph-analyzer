/*
 * Filename		: GraphHash.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#include "GraphHash.h"

GraphHash::GraphHash()
{
    m_name = "GraphHash";
    m_description = "Hashes whole graph for further comparison.";
    m_traitName = "hash";
}

bool GraphHash::perform(ExploitSample *sample)
{
    Graph *g = sample->graph();
	Graph::graph_iterator it;
    QMap<QString, QString> *m = new QMap<QString, QString>();
    QString graphString = "";
	InstructionSplitter splitter;
	instr_vertex *iv;
	for(it = g->begin(); it != g->end(); ++it) {
		iv = (instr_vertex *) it->data;
		splitter = emu_string_char(iv->instr_string);
        graphString += splitter.code();
        graphString += splitter.firstArg();
        graphString += splitter.secondArg();
        graphString += splitter.syscall();
		splitter.clear();
	}

    (*m)["hash"] = Toolbox::hash(graphString);
    LOG("hash: [%s]\n", Toolbox::hash(graphString).toStdString().c_str());

	/* set traits */
    sample->info()->setTrait(m_traitName, m);

    LOG("SUCCESS\n\n");
	return true;
}
