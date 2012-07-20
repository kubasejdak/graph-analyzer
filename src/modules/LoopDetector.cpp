/*
 * Filename	: LoopDetector.cpp
 * Author	: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#include "LoopDetector.h"

LoopDetector::LoopDetector() {
	id = getNextID();
	name = "LoopDetector";
	description = "Detects loops and returns beginning address and number of iterations.";
}

LoopDetector::~LoopDetector() {
}

bool LoopDetector::perform(ShellcodeSample *sample) {
	Graph *g = sample->getGraph();
	Graph::graph_iterator it;
	map<string, string> *m;
	emu_edge *e;
	int n = 0, iter = 0;
	for(it = g->begin(); it != g->end(); ++it) {
		n = g->detectLoop(it);
		if(n) {
			for(e = emu_edges_first(it->edges); !emu_edges_attail(e); e = emu_edges_next(e)) {
				if(e->count > 1) {
					iter = e->count;
					break;
				}
			}

			instr_vertex *iv = (instr_vertex *) it->data;
			m = new map<string, string>();
			(*m)["start"] = itos(iv->eip, true);
			(*m)["iterations"] = itos(iter);
			(*m)["vertexes"] = itos(n);
			sample->getInfo()->setTrait("loop", m);
		}
	}

	return true;
}
