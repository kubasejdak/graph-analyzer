/*
 * Filename	: DecoderKey.cpp
 * Author	: Kuba Sejdak
 * Created on	: 12-07-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "DecoderKey.h"

DecoderKey::DecoderKey() {
	id = getNextID();
	name = "DecoderKey";
	description = "Searches for decoding block and extracts XOR key.";
}

DecoderKey::~DecoderKey() {
}

bool DecoderKey::perform(ShellcodeSample *sample) {
	Graph *g = sample->getGraph();
	Graph::graph_iterator it;
	InstructionSplitter splitter;
	for(it = g->begin(); it != g->end(); ++it) {
		instr_vertex *iv = (instr_vertex *) it->data;
		splitter = emu_string_char(iv->instr_string);
	}

	return true;
}
