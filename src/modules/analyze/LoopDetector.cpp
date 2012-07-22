/*
 * Filename	: LoopDetector.cpp
 * Author	: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#define LOCAL_DEBUG
#include <debug.h>

#include "LoopDetector.h"

LoopDetector::LoopDetector() {
	id = getNextID();
	name = "LoopDetector";
	description = "Detects loops and returns detailed information.";
}

LoopDetector::~LoopDetector() {
}

bool LoopDetector::perform(ShellcodeSample *sample) {
	Graph *g = sample->getGraph();
	Graph::graph_iterator it;
	loop_container *loops;
	loop_vec *vec;
	instr_vertex *iv;
	map<string, string> *m;
	InstructionSplitter splitter;
	string vertexes;
	for(it = g->begin(); it != g->end(); ++it) {
		loops = g->detectLoop(it);
		if(loops == NULL)
			continue;

		/* extract information about loops */
		for(unsigned i = 0; i < loops->size(); ++i) {
			vec = (*loops)[i];
			iv = (instr_vertex *) vec->front()->data;
			m = new map<string, string>();

			/* start address */
			(*m)["start"] = itos(iv->eip, true);

			/* number of vertexes */
			(*m)["size"] = itos(vec->size());

			/* list of vertexes */
			vertexes = "\n\t";
			for(unsigned j = 0; j < vec->size(); ++j) {
				if(j % 12 == 0 && j != 0)
					vertexes += "\n\t";

				iv = (instr_vertex *) vec->at(j)->data;
				vertexes += itos(iv->eip, true);
				vertexes += ", ";
			}
			(*m)["vertexes"] = vertexes;

			/* number of iterations */
			emu_edge *e = emu_edges_first(vec->back()->edges);
			for(; !emu_edges_attail(e); e = emu_edges_next(e)) {
				iv = (instr_vertex *) e->destination->data;
				if(itos(iv->eip, true) == (*m)["start"]) {
					(*m)["iterations"] = itos(e->count);
					break;
				}

			}

			/* loop hash */
			string loop_string = "";
			for(unsigned k = 0; k < vec->size(); ++k) {
				iv = (instr_vertex *) vec->at(i)->data;
				splitter = emu_string_char(iv->instr_string);
				loop_string += splitter.getCode();
				loop_string += splitter.getFirstArg();
				loop_string += splitter.getSecondArg();
				loop_string += splitter.getSyscall();
			}

			string h = hash(loop_string);
			(*m)["hash"] = h;

			/* set traits */
			sample->getInfo()->setTrait("loop", m);
		}

		/* delete returned container */
		for(unsigned i = 0; i < loops->size(); ++i)
			delete (*loops)[i];

		delete loops;
	}

	return true;
}
