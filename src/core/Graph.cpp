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

loop_container *Graph::detectLoop(Graph::graph_iterator from_it) {
	emu_vertex *from = &(*from_it);

	/* eliminate impossible loop starts */
	uint32_t n = (emu_vertexes_ishead(from)) ? 1 : 2;
	if(from->backlinks < n)
		return NULL;

	loop_container *loops = new loop_container();
	loop_vec *vec = new loop_vec();
	loop_vec *tmp = new loop_vec();

	vec->push_back(from);
	loops->push_back(vec);

	emu_edge *e;
	emu_edge *tmp_e = emu_edge_new();
	emu_vertex *v;

	/* possible loop paths for vertex "from" */
	for(unsigned i = 0; i < loops->size(); ++i) {
		/* last vertex in actual path */
		v = (*loops)[i]->back();

		/* clear colors starting from this vertex */
		clearVertColor(v);

		/* all vertexes in possible loop path */
		while(v != NULL) {
			/* save actual path */
			*tmp = *((*loops)[i]);

			/* this vertex is visited */
			v->color = black;

			/* all edges from v */
			bool actual = 1;
			bool continuation = true;
			bool has_edges = false;
			for(e = emu_edges_first(v->edges); !emu_edges_attail(e); e = emu_edges_next(e), actual = 0) {
				/* at least one edge */
				has_edges = true;

				if(actual) {
					if(e->destination == from) {
						continuation = false;
						continue;
					}
					else if(e->destination->color == black) {
						continuation = false;
						continue;
					}
					else {
						*tmp_e = *e;
						(*loops)[i]->push_back(e->destination);
					}
				}
				else {
					if(e->destination->color == black)
						continue;
					else {
						vec = new loop_vec();
						*vec = *tmp;
						vec->push_back(e->destination);
						loops->push_back(vec);
					}
				}
			}

			if(!continuation || !has_edges)
				v = NULL;

			if(v == NULL || emu_vertexes_istail(v))
				break;

			v = tmp_e->destination;
		} /* while */

		/* remove paths that are not loops */
		v = (*loops)[i]->back();
		bool loop_found = false;
		for(e = emu_edges_first(v->edges); !emu_edges_attail(e); e = emu_edges_next(e)) {
			if(e->destination == from) {
				loop_found = true;
				break;
			}
		}

		if(!loop_found) {
			delete (*loops)[i];
			loops->erase(loops->begin() + i);
			--i;
		}
	} /* for */

	return loops;
}

void Graph::clearVertColor(emu_vertex *from) {
	for(graph_iterator it(this, from); it != end(); ++it)
		it->color = white;
}
