/*
 * Filename		: Dot.cpp
 * Author		: Kuba Sejdak
 * Created on	: 21-06-2012
 */

#include "Dot.h"

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
	struct instr_vertex *iv = (struct instr_vertex *) malloc(
			sizeof(struct instr_vertex));
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

int graph_draw(struct emu_graph *graph)
{
	struct emu_vertex *ev;
	struct instr_vertex *iv;

	FILE *f = fopen("graph.dot", "w+");

	struct emu_graph *sgraph = emu_graph_new();
	struct emu_hashtable *ht = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);

	struct emu_vertex *nev;
	struct instr_vertex *niv = NULL;

	LOG("copying vertexes\n");
	for(ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev)) {
		iv = (struct instr_vertex *) ev->data;

		nev = emu_vertex_new();
		emu_graph_vertex_add(sgraph, nev);

		niv = instr_vertex_copy(iv);
		nev->data = niv;

		emu_hashtable_insert(ht, (void *) iv, nev);
		ev->color = white;
	}

	LOG("optimizing graph\n");
	for(ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev)) {
		/* ignore known */
		if(ev->color == black)
			continue;

		LOG_ERROR("vertex %p\n", (void *) ev);

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
			LOG(" -> vertex %p\n", (void *) ev);
		}

		iv = (struct instr_vertex *) ev->data;

		/* create the new vertex */
		nev = (struct emu_vertex *) emu_hashtable_search(ht, (void *) iv)->value;
		niv = (struct instr_vertex *) nev->data;
		iv = (struct instr_vertex *) ev->data;

		LOG("going forwards from %p\n", (void *) ev);
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
			LOG(" -> vertex %p\n", (void *) ev);
		}

		ev->color = black;

		LOG("copying edges for %p\n", (void *) ev);
		struct emu_edge *ee;
		for(ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee)) {
			struct instr_vertex *ivto = (instr_vertex *) emu_vertex_data_get(ee->destination);
			struct emu_hashtable_item *ehi = emu_hashtable_search(ht, (void *) ivto);
			struct emu_vertex *to = (struct emu_vertex *) ehi->value;
			if(1) { // nev != to )//&& to->color != black )
				struct emu_edge *nee = emu_vertex_edge_add(nev, to);
				nee->count = ee->count;
				nee->data = ee->data;
				LOG(" -> %p\n", (void *) to);
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

	return 0;
}
