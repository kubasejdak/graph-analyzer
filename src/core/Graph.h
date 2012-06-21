/*
 * Filename	: Graph.h
 * Author	: Kuba Sejdak
 * Created on	: 20-06-2012
 */

#ifndef GRAPH_H_
#define GRAPH_H_

/* libemu headers */
extern "C" {
	#include <emu/emu_graph.h>
	#include <emu/emu_hashtable.h>
}

class Graph {
public:
	Graph();
	virtual ~Graph();

	emu_graph *getEmuGraph();
	emu_hashtable *getEmuHashtable();

private:
	struct emu_graph *graph;
	struct emu_hashtable *hashtable;
};

#endif /* GRAPH_H_ */
