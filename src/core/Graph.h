/*
 * Filename		: Graph.h
 * Author		: Kuba Sejdak
 * Created on	: 20-06-2012
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <iterator>
#include <QVector>
#include <QString>
using namespace std;

extern "C" {
	#include <emu/emu_hashtable.h>
	#include <emu/emu_queue.h>
	#include <emu/emu.h>
	#include <emu/emu_memory.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_cpu_data.h>
	#include <emu/emu_cpu_stack.h>
	#include <emu/emu_string.h>
	#include <emu/emu_graph.h>
	#include <emu/environment/emu_profile.h>
	#include <emu/environment/emu_env.h>
	#include <emu/environment/linux/emu_env_linux.h>
	#include <emu/environment/win32/emu_env_w32.h>
	#include <emu/environment/win32/emu_env_w32_dll.h>
	#include <emu/environment/win32/emu_env_w32_dll_export.h>
	#include <emu/environment/win32/env_w32_dll_export_kernel32_hooks.h>
	#include <emu/environment/linux/env_linux_syscall_hooks.h>
}

#include <utils/InstructionSplitter.h>
#include <utils/SystemLogger.h>

//====================================================================================================================================

struct instr_vertex {
	uint32_t eip;
	struct emu_string  *instr_string;
	struct emu_env_w32_dll *dll;
	struct emu_env_linux_syscall *syscall;
};


struct instr_vertex *instr_vertex_new(uint32_t theeip, const char *instr_string);
void instr_vertex_free(struct instr_vertex *iv);
struct instr_vertex *instr_vertex_copy(struct instr_vertex *from);
void instr_vertex_destructor(void *data);

//====================================================================================================================================

typedef QVector<emu_vertex *> LoopVec;
typedef QVector<LoopVec *> LoopContainer;

enum GraphExportStrategy {
	DOT_FILE = 0,
	XML_FILE
};

class Graph {
public:
	Graph();
	Graph(struct emu_graph *g, struct emu_hashtable *h);
	virtual ~Graph();

    emu_graph *emuGraph();
    emu_hashtable *emuHashtable();

	/* iterator */
	class graph_iterator : public std::iterator<forward_iterator_tag, emu_vertex> {
	public:
		graph_iterator();
		graph_iterator(Graph *g, emu_vertex *v);
		graph_iterator(const graph_iterator &it);

		graph_iterator &operator++();
		graph_iterator &operator--();
		bool operator==(const graph_iterator &r);
		bool operator!=(const graph_iterator &r);
		emu_vertex &operator*();
		emu_vertex *operator->();

	private:
        emu_vertex *m_outVertex;
        Graph *m_outGraph;
		friend class Graph;
	}; /* iterator */

	graph_iterator begin();
	graph_iterator end();

    LoopContainer *detectLoop(graph_iterator from_it);
	int size();

	bool exportGraph(GraphExportStrategy strategy, QString filename);

private:
	void clearVertColor(emu_vertex *from);

	bool dotExportStrategy(QString filename);

    struct emu_graph *m_graph;
    struct emu_hashtable *m_hashtable;
};

#endif /* GRAPH_H_ */
