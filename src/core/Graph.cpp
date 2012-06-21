/*
 * Filename	: Graph.cpp
 * Author	: Kuba Sejdak
 * Created on	: 20-06-2012
 */

#include "Graph.h"

Graph::Graph() {
	graph = emu_graph_new();
	hashtable = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);
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
