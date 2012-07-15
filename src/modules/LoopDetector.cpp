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
	Graph::graph_iterator it, it2;
	int n;
	string loop;
	for(it = g->begin(); it != g->end(); ++it) {
		n = g->detectLoop(it);
		if(n > 1) {
			instr_vertex *iv = (instr_vertex *) it->data;
			loop = mergeLoopDesc(iv->eip, n);
			sample->getInfo()->setTrait("loop", loop);
		}
	}

	return true;
}

string LoopDetector::mergeLoopDesc(int address, int iter) {
	stringstream s1, s2;
	s1 << hex << address;
	string ret = "0x";
	ret += s1.str();
	ret += " (";
	s2 << iter;
	ret += s2.str();
	ret += ")";

	return ret;
}
