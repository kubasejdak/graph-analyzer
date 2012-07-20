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
	/* eliminate impossible loop starts */
	uint32_t n = (emu_vertexes_ishead(&(*it))) ? 1 : 2;
	if(it->backlinks < n)
		return 0;

	/* reset color and distance */
	graph_iterator i;
	for(i = it; i != end(); ++i) {
		i->color = white;
		i->distance = 0;
	}

	/* look for loops */
	emu_vertex *v;
	emu_edge *e;
	emu_queue *queue = emu_queue_new();
	emu_queue_enqueue(queue, &(*it));

	while(!emu_queue_empty(queue)) {
		v = (emu_vertex *) emu_queue_dequeue(queue);
		for(e = emu_edges_first(v->edges); !emu_edges_attail(e); e = emu_edges_next(e)) {
			if(e->destination == &(*it)) {
				emu_queue_free(queue);
				return v->distance + 1;
			}

			if(e->destination->color != white)
				continue;

			e->destination->distance = v->distance + 1;
			e->destination->color = grey;
			emu_queue_enqueue(queue, e->destination);
		}

		v->color = black;
	}

	emu_queue_free(queue);
	return 0;
}
