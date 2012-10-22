/*
 * Filename		: Graph.cpp
 * Author		: Kuba Sejdak
 * Created on	: 20-06-2012
 */

#include "Graph.h"

Graph::Graph()
{
    m_graph = emu_graph_new();
    m_hashtable = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);
}

Graph::Graph(struct emu_graph *g, struct emu_hashtable *h)
{
    m_graph = g;
    m_hashtable = h;
}

Graph::~Graph()
{
    emu_hashtable_free(m_hashtable);
    emu_graph_free(m_graph);
}

emu_graph *Graph::emuGraph()
{
    return m_graph;
}

emu_hashtable *Graph::emuHashtable()
{
    return m_hashtable;
}

Graph::graph_iterator::graph_iterator()
{
    m_outGraph = NULL;
    m_outVertex = NULL;
}

Graph::graph_iterator::graph_iterator(Graph *g, emu_vertex *v)
{
    m_outGraph = g;
    m_outVertex = v;
}

Graph::graph_iterator::graph_iterator(const graph_iterator &it)
{
    m_outGraph = it.m_outGraph;
    m_outVertex = it.m_outVertex;
}

Graph::graph_iterator &Graph::graph_iterator::operator++()
{
    if(!m_outVertex || *this == m_outGraph->end())
		return *this;

    m_outVertex = emu_vertexes_next(m_outVertex);
	return *this;
}

Graph::graph_iterator &Graph::graph_iterator::operator--()
{
    if(!m_outVertex || *this == m_outGraph->begin())
		return *this;

    m_outVertex = emu_vertexes_prev(m_outVertex);
	return *this;
}

bool Graph::graph_iterator::operator==(const graph_iterator &r)
{
    return m_outVertex == r.m_outVertex;
}

bool Graph::graph_iterator::operator!=(const graph_iterator &r)
{
    return m_outVertex != r.m_outVertex;
}

emu_vertex &Graph::graph_iterator::operator*()
{
    return *m_outVertex;
}

emu_vertex *Graph::graph_iterator::operator->()
{
    return m_outVertex;
}

Graph::graph_iterator Graph::begin()
{
    if(emu_vertexes_empty(m_graph->vertexes))
		return end();

    return graph_iterator(this, emu_vertexes_first(m_graph->vertexes));
}

Graph::graph_iterator Graph::end()
{
    if(emu_vertexes_empty(m_graph->vertexes))
		return graph_iterator(this, NULL);

    return graph_iterator(this, emu_vertexes_last(m_graph->vertexes));
}

LoopContainer *Graph::detectLoop(Graph::graph_iterator from_it)
{
	emu_vertex *from = &(*from_it);

	/* eliminate impossible loop starts */
	uint32_t n = (emu_vertexes_ishead(from)) ? 1 : 2;
	if(from->backlinks < n)
		return NULL;

    LoopContainer *loops = new LoopContainer();
    LoopVec *vec = new LoopVec();
    LoopVec *tmp = new LoopVec();

	vec->push_back(from);
	loops->push_back(vec);

	emu_edge *e;
	emu_edge *tmp_e = emu_edge_new();
	emu_vertex *v;

	/* possible loop paths for vertex "from" */
    for(int i = 0; i < loops->size(); ++i) {
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
                        vec = new LoopVec();
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

void Graph::clearVertColor(emu_vertex *from)
{
	for(graph_iterator it(this, from); it != end(); ++it)
		it->color = white;
}
