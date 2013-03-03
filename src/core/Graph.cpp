/*
 * Filename		: Graph.cpp
 * Author		: Kuba Sejdak
 * Created on	: 20-06-2012
 */

#include "Graph.h"

//====================================================================================================================================

struct instr_vertex *instr_vertex_new(uint32_t theeip, const char *instr_string)
{
	struct instr_vertex *iv = (struct instr_vertex *) malloc(sizeof(struct instr_vertex));
	memset(iv, 0, sizeof(struct instr_vertex));
	iv->eip = theeip;
	iv->instr_string = emu_string_new();
	emu_string_append_format(iv->instr_string, "0x%08x %s\\l", iv->eip, instr_string);
	return iv;
}

void instr_vertex_free(struct instr_vertex *iv)
{
	emu_string_free(iv->instr_string);
	free(iv);
}

struct instr_vertex *instr_vertex_copy(struct instr_vertex *from)
{
	struct instr_vertex *iv = (struct instr_vertex *) malloc(sizeof(struct instr_vertex));
	memset(iv, 0, sizeof(struct instr_vertex));
	iv->eip = from->eip;
	iv->instr_string = emu_string_new();
	iv->dll = from->dll;
	iv->syscall = from->syscall;

	emu_string_append_char(iv->instr_string, (char *) from->instr_string->data);
	return iv;
}

void instr_vertex_destructor(void *data)
{
	instr_vertex_free((struct instr_vertex *) data);
}

//====================================================================================================================================

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
		while(true) {
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

			if(!continuation || !has_edges || emu_vertexes_istail(v))
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

int Graph::size()
{
	int vertexes_num = 0;
	for(graph_iterator it = begin(); it != end(); ++it)
		++vertexes_num;

	return vertexes_num;
}

bool Graph::exportGraph(GraphExportStrategy strategy, QString filename)
{
	bool stat = false;

	switch(strategy) {
	case DOT_FILE:
		stat = dotExportStrategy(filename);
		break;
	case XML_FILE:
		break;
	}

	return stat;
}

bool Graph::dotExportStrategy(QString filename)
{
	struct emu_vertex *ev;
	struct instr_vertex *iv;

	FILE *f = fopen(filename.toStdString().c_str(), "w+");
	if(f == NULL) {
		LOG_ERROR("cannot open temporary file [%s]\n", filename.toStdString().c_str());
		return false;
	}

	struct emu_graph *graph = m_graph;
	struct emu_graph *sgraph = emu_graph_new();
	struct emu_hashtable *ht = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);

	struct emu_vertex *nev;
	struct instr_vertex *niv = NULL;

	for(ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev)) {
		iv = (struct instr_vertex *) ev->data;

		nev = emu_vertex_new();
		emu_graph_vertex_add(sgraph, nev);

		niv = instr_vertex_copy(iv);
		nev->data = niv;

		emu_hashtable_insert(ht, (void *) iv, nev);
		ev->color = white;
	}

	for(ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev)) {
		/* ignore known */
		if(ev->color == black)
			continue;

		/* find the first in a chain */
		iv = (struct instr_vertex *) ev->data;
		while(emu_edges_length(ev->backedges) == 1 && emu_edges_length(ev->edges) <= 1
				&& ev->color == white && iv->dll == NULL && iv->syscall == NULL) {
			ev->color = grey;

			struct emu_vertex *xev = emu_edges_first(ev->backedges)->destination;
			iv = (struct instr_vertex *) xev->data;
			if(emu_edges_length(xev->backedges) > 1 || emu_edges_length(xev->edges) > 1 || iv->dll != NULL || iv->syscall != NULL)
				break;

			ev = xev;
		}

		iv = (struct instr_vertex *) ev->data;

		/* create the new vertex */
		nev = (struct emu_vertex *) emu_hashtable_search(ht, (void *) iv)->value;
		niv = (struct instr_vertex *) nev->data;
		iv = (struct instr_vertex *) ev->data;

		while(emu_edges_length(ev->edges) == 1 && emu_edges_length(ev->backedges) <= 1
				&& ev->color != black && iv->dll == NULL && iv->syscall == NULL) {
			ev->color = black;
			struct emu_vertex *xev = emu_edges_first(ev->edges)->destination;
			iv = (struct instr_vertex *) xev->data;

			if(emu_edges_length(xev->backedges) > 1 || emu_edges_length(
				xev->edges) > 1 || iv->dll != NULL || iv->syscall != NULL)
				break;

			ev = xev;
			iv = (struct instr_vertex *) ev->data;
			emu_string_append_char(niv->instr_string, emu_string_char(iv->instr_string));
		}

		ev->color = black;

		struct emu_edge *ee;
		for(ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee)) {
			struct instr_vertex *ivto = (instr_vertex *) emu_vertex_data_get(ee->destination);
			struct emu_hashtable_item *ehi = emu_hashtable_search(ht, (void *) ivto);
			struct emu_vertex *to = (struct emu_vertex *) ehi->value;
			if(1) { // nev != to )//&& to->color != black )
				struct emu_edge *nee = emu_vertex_edge_add(nev, to);
				nee->count = ee->count;
				nee->data = ee->data;
			}
		}
	}

	graph->vertex_destructor = instr_vertex_destructor;
	graph = sgraph;

	emu_hashtable_free(ht);
	fprintf(f, "digraph G {\n\t//rankdir=LR\n\tnode [fontname=Courier, labeljust=r];\n");

	for(ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev)) {
		if(emu_edges_length(ev->edges) == 0 && emu_edges_length(ev->backedges) == 0)
			continue;

		struct instr_vertex *iv = (instr_vertex *) emu_vertex_data_get(ev);

		if(iv->dll != NULL || iv->syscall != NULL)
			fprintf(f, "\t \"%p\" [shape=box, style=filled, color=\".7 .3 1.0\", label = \"%s\"]\n", (void *) iv, emu_string_char(iv->instr_string));
		else
			fprintf(f, "\t \"%p\" [shape=box, label = \"%s\"]\n", (void *) iv, emu_string_char(iv->instr_string));
	}

	for(ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev)) {
		struct instr_vertex *ivfrom = (instr_vertex *) emu_vertex_data_get(ev);

		struct emu_edge *ee;
		for(ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee)) {
			struct instr_vertex *ivto = (instr_vertex *) emu_vertex_data_get(ee->destination);
			struct emu_string *fs = emu_string_new();

			if(ee->data != (void *) 0x0)
				emu_string_append_format(fs, "\t \"%p\" -> \"%p\" [style = dashed", (void *) ivfrom, (void *) ivto);
			else
				emu_string_append_format(fs, "\t \"%p\" -> \"%p\" [style = bold", (void *) ivfrom, (void *) ivto);

			if(ee->count > 100)
				emu_string_append_char(fs, ", color=red");
			else if(ee->count > 50)
				emu_string_append_char(fs, ", color=blue");
			else if(ee->count > 25)
				emu_string_append_char(fs, ", color=green");
			else if(ee->count > 1)
				emu_string_append_char(fs, ", color=orange");

			emu_string_append_char(fs, " ]\n");

			fprintf(f, "%s", emu_string_char(fs));
			emu_string_free(fs);
		}
	}

	fprintf(f, "}");
	fclose(f);

	graph->vertex_destructor = instr_vertex_destructor;
	emu_graph_free(sgraph);

	LOG("SUCCESS\n\n");
	return true;
}
