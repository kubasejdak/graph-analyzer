/*
 * Filename	: GraphHash.cpp
 * Author	: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#include "GraphHash.h"

GraphHash::GraphHash() {
	id = getNextID();
	name = "GraphHash";
	description = "Hashes whole graph for further comparison.";
	trait_name = "hash";
}

GraphHash::~GraphHash() {
}

bool GraphHash::perform(ShellcodeSample *sample) {
	Graph *g = sample->getGraph();
	Graph::graph_iterator it;
	map<string, string> *m = new map<string, string>();
	string graph_string = "";
	InstructionSplitter splitter;
	instr_vertex *iv;
	for(it = g->begin(); it != g->end(); ++it) {
		iv = (instr_vertex *) it->data;
		splitter = emu_string_char(iv->instr_string);
		graph_string += splitter.getCode();
		graph_string += splitter.getFirstArg();
		graph_string += splitter.getSecondArg();
		graph_string += splitter.getSyscall();
		splitter.clear();
	}

	(*m)["hash"] = hash(graph_string);

	/* set traits */
	sample->getInfo()->setTrait(trait_name, m);

	return true;
}
