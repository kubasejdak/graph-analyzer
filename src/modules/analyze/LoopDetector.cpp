/*
 * Filename		: LoopDetector.cpp
 * Author		: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#include "LoopDetector.h"

LoopDetector::LoopDetector()
{
    m_name = "LoopDetector";
    m_description = "Detects loops and returns detailed information.";
    m_traitName = "loop";
}

bool LoopDetector::perform(ShellcodeSample *sample)
{
    LOG("\n");
    Graph *g = sample->graph();
	Graph::graph_iterator it;
    LoopContainer *loops;
    LoopVec *vec;
	instr_vertex *iv;
    QMap<QString, QString> *m;
	InstructionSplitter splitter;
    QString vertexes;
	for(it = g->begin(); it != g->end(); ++it) {
		loops = g->detectLoop(it);
		if(loops == NULL)
			continue;

		/* extract information about loops */
        LOG("loop found\n");
        for(int i = 0; i < loops->size(); ++i) {
			vec = (*loops)[i];
			iv = (instr_vertex *) vec->front()->data;
            m = new QMap<QString, QString>();

			/* start address */
            (*m)["start"] = Toolbox::itos(iv->eip, 16);
            LOG("start: [%s]\n", Toolbox::itos(iv->eip).toStdString().c_str());

			/* number of vertexes */
            (*m)["size"] = Toolbox::itos(vec->size());
            LOG("size: [%s]\n", Toolbox::itos(vec->size()).toStdString().c_str());

			/* list of vertexes */
			vertexes = "";
            for(int j = 0; j < vec->size(); ++j) {
				iv = (instr_vertex *) vec->at(j)->data;
                vertexes += Toolbox::itos(iv->eip, 16);
				if(j != vec->size() - 1)
					vertexes += ", ";
			}
			(*m)["vertexes"] = vertexes;
            LOG("vertexes: [%s]\n", vertexes.toStdString().c_str());

			/* number of iterations */
			emu_edge *e = emu_edges_first(vec->back()->edges);
			for(; !emu_edges_attail(e); e = emu_edges_next(e)) {
				iv = (instr_vertex *) e->destination->data;
                if(Toolbox::itos(iv->eip, 16) == (*m)["start"]) {
                    (*m)["iterations"] = Toolbox::itos(e->count);
                    LOG("iterations: [%s]\n", Toolbox::itos(e->count).toStdString().c_str());
					break;
				}

			}

			/* loop hash */
            QString loopString = "";
            for(int k = 0; k < vec->size(); ++k) {
				iv = (instr_vertex *) vec->at(i)->data;
				splitter = emu_string_char(iv->instr_string);
                loopString += splitter.code();
                loopString += splitter.firstArg();
                loopString += splitter.secondArg();
                loopString += splitter.syscall();
				splitter.clear();
			}

            (*m)["hash"] = Toolbox::hash(loopString);

			/* set traits */
            sample->info()->setTrait(m_traitName, m);
		}

		/* delete returned container */
        for(int i = 0; i < loops->size(); ++i)
			delete (*loops)[i];

		delete loops;
	}

    LOG("SUCCESS\n");
	return true;
}
