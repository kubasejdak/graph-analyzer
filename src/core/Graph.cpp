/*
 * Filename	: Graph.cpp
 * Author	: Kuba Sejdak
 * Created on	: 20-06-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "Graph.h"

Graph::Graph() {
	graph = emu_graph_new();
	hashtable = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);
}

Graph::Graph(struct emu_graph *g, struct emu_hashtable *h) {
	graph = g;
	hashtable = h;
}

Graph::~Graph() {
	emu_hashtable_free(hashtable);
	emu_graph_free(graph);
}

emu_graph *Graph::getEmuGraph() {
	return graph;
}

emu_hashtable *Graph::getEmuHashtable() {
	return hashtable;
}

Graph::graph_iterator::graph_iterator() {
	outGraph = NULL;
	outVertex= NULL;
}

Graph::graph_iterator::graph_iterator(Graph *g, emu_vertex *v) {
	outGraph = g;
	outVertex= v;
}

Graph::graph_iterator::graph_iterator(const graph_iterator &it) {
	outGraph = it.outGraph;
	outVertex = it.outVertex;
}

Graph::graph_iterator &Graph::graph_iterator::operator++() {
	if(!outVertex || *this == outGraph->end())
		return *this;

	outVertex = emu_vertexes_next(outVertex);
		return *this;
}

Graph::graph_iterator &Graph::graph_iterator::operator--() {
	if(!outVertex || *this == outGraph->begin())
		return *this;

	outVertex = emu_vertexes_prev(outVertex);
		return *this;
}

bool Graph::graph_iterator::operator==(const graph_iterator &r) {
	return outVertex == r.outVertex;
}

bool Graph::graph_iterator::operator!=(const graph_iterator &r) {
	return outVertex != r.outVertex;
}

emu_vertex &Graph::graph_iterator::operator*() {
	return *outVertex;
}

emu_vertex *Graph::graph_iterator::operator->() {
	return outVertex;
}

Graph::graph_iterator Graph::begin() {
	if(emu_vertexes_empty(graph->vertexes))
		return end();

	return graph_iterator(this, emu_vertexes_first(graph->vertexes));
}

Graph::graph_iterator Graph::end() {
	if(emu_vertexes_empty(graph->vertexes))
		return graph_iterator(this, NULL);

	return graph_iterator(this, emu_vertexes_last(graph->vertexes));
}

int Graph::detectLoop(Graph::graph_iterator it) {
	emu_edge_root *r = it->edges;
	emu_edge *e;
	for(e = emu_edges_first(r); !emu_edges_istail(e); e = emu_edges_next(e)) {
		if(e->count > 1 && it->backlinks > 1 && hasLoopEnd(it))
			return e->count;
	}

	return 1;
}

bool Graph::hasLoopEnd(Graph::graph_iterator it) {
	instr_vertex *v;;
	emu_edge_root *r = it->backedges;
	emu_edge *e;
	InstructionSplitter splitter;
	string instr;

	string loopInstr[9];
	loopInstr[0] = "loop";
	loopInstr[1] = "ret";
	loopInstr[2] = "rep";
	loopInstr[3] = "jmp";
	loopInstr[4] = "jnz";
	loopInstr[5] = "jne";
	loopInstr[6] = "jz";
	loopInstr[7] = "je";
	loopInstr[8] = "call";

	for(e = emu_edges_first(r); !emu_edges_istail(e); e = emu_edges_next(e)) {
		v = (instr_vertex *) e->destination->data;
		splitter = emu_string_char(v->instr_string);
		instr = splitter.getInstr();
		for(unsigned int i = 0; i < sizeof(loopInstr); ++i)
			if(instr == loopInstr[i])
				return true;
	}

	return false;
}
