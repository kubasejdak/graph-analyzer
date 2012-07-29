/*
 *  Filename	: EmulationSystem.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

/* debug */
//#define LOCAL_DEBUG
#include <debug.h>

#include "EmulationSystem.h"

EmulationSystem::EmulationSystem() {
	emuUnit = new EmulationUnit();
	sample = NULL;
}

EmulationSystem::~EmulationSystem() {
	delete emuUnit;
}

void EmulationSystem::loadSample(ShellcodeSample *sample) {
	this->sample = sample;
}

bool EmulationSystem::emulate() {
	if(!sample)
		return false;

	/* load code to emu unit */
	int32_t codeOffset;
	codeOffset = emuUnit->loadCode(sample->getCode(), sample->getInfo()->getSize());
	sample->getInfo()->setCodeOffset(codeOffset);
	sample->getInfo()->setShellcodePresent(codeOffset >= 0 ? true : false);

	/* if exploit not detected return */
	if(!sample->getInfo()->isShellcodePresent()) {
		sample = NULL;
		return true;
	}

	/* prepare environment */
	uint32_t eipsave = 0;
	int ret = 0;

	Graph *graph = sample->getGraph();
	struct emu_cpu *cpu = emuUnit->getCpu();
	struct emu_env *env = emuUnit->getEnv();
	struct emu_graph *eg = graph->getEmuGraph();
	struct emu_hashtable *eh = graph->getEmuHashtable();
	struct emu_vertex *last_vertex = NULL;
	struct emu_vertex *ev = NULL;
	struct emu_hashtable_item *ehi = NULL;
	struct emu_env_hook *hook = NULL;
	struct instr_vertex *iv = NULL;

	/* turn on disassembling */
	emu_cpu_debugflag_set(cpu, instruction_string);

	/* emulation loop */
	for(int i = 0; i < EMULATION_STEPS; ++i) {
		if(!cpu->repeat_current_instr)
			eipsave = emu_cpu_eip_get(cpu);

		/* initialize variables per iteration */
		hook = NULL;
		ev = NULL;
		iv = NULL;
		ret = 0;

		/* check if there is any vertex with actual eip */
		ehi = emu_hashtable_search(eh, (void *)(uintptr_t) eipsave);
		if(ehi != NULL)
			ev = (struct emu_vertex *) ehi->value;

		/* create new vertex if eip is unique */
		if(ev == NULL) {
			ev = emu_vertex_new();
			emu_graph_vertex_add(eg, ev);
			emu_hashtable_insert(eh, (void *)(uintptr_t) eipsave, ev);
		}

		/* check if we are in any dll */
		hook = emu_env_w32_eip_check(env);

		/* if yes */
		if(hook != NULL) {
			if(ev->data != NULL && strcmp(hook->hook.win->fnname, "CreateProcessA") == 0) {
				ev = emu_vertex_new();
				emu_graph_vertex_add(eg, ev);
			}

			/* save syscall from dll in vertex */
			iv = instr_vertex_new(eipsave, hook->hook.win->fnname);
			emu_vertex_data_set(ev, iv);

			/* get the dll */
			int numdlls = 0;
			while(env->env.win->loaded_dlls[numdlls] != NULL) {
				if(eipsave > env->env.win->loaded_dlls[numdlls]->baseaddr &&
					 eipsave < env->env.win->loaded_dlls[numdlls]->baseaddr + env->env.win->loaded_dlls[numdlls]->imagesize )
				{
					iv->dll = env->env.win->loaded_dlls[numdlls];
				}
				++numdlls;
			}

			/* no function defined fot this name */
			if(hook->hook.win->fnhook == NULL) {
				PRINTMSG("unhooked call to %s", hook->hook.win->fnname);
				return false;
			}
		}
		/* if not in any dll */
		else {
			ret = emu_cpu_parse(cpu);

			struct emu_env_hook *hook =NULL;
			if(ret != -1) {
				if((hook = emu_env_linux_syscall_check(env)) != NULL) {
					if(ev->data == NULL) {
						iv = instr_vertex_new(eipsave, hook->hook.lin->name);
						emu_vertex_data_set(ev, iv);
						iv->syscall = hook->hook.lin;
					}
				}
				else {
					if(ev->data == NULL) {
						/* save instr string in vertex */
						iv = instr_vertex_new(eipsave, cpu->instr_string);
						emu_vertex_data_set(ev, iv);
					}
				}
			}
			else {
				if(ev->data == NULL) {
					iv = instr_vertex_new(eipsave, "ERROR");
					emu_vertex_data_set(ev, iv);
				}
			}

			if(ret != -1) {
				if(hook == NULL)
					ret = emu_cpu_step(cpu);
				else {
					if(hook->hook.lin->fnhook != NULL)
						hook->hook.lin->fnhook(env, hook);
					else
						return false;
				}
			}

			if(ret == -1) {
				/* step failed - maybe SEH */
				if(emu_env_w32_step_failed(env) != 0) {
					PRINTMSG("cpu %s", emu_strerror(emuUnit->getEmu()));
					break;
				}
			}
			/* in case of SEH exception */
			else
				env->env.win->last_good_eip = emu_cpu_eip_get(cpu);
		} /* else dla if(hook != NULL) */

		/* link two last vertexes with an edge */
		if(last_vertex != NULL) {
			struct emu_edge *ee = emu_vertex_edge_add(last_vertex, ev);
			if(cpu->instr.is_fpu == 0 && cpu->instr.source.cond_pos == eipsave && cpu->instr.source.has_cond_pos == 1)
				ee->data = (void *) 0x1;
		}
		last_vertex = ev;
	} /* emulation loop */

	/* create .dot file */
	graph_draw(graph->getEmuGraph());

	/* draw graph using dot package and sample name */
	string graphName = sample->getInfo()->getName();
	graphName = trimExtension(graphName);
	graphName += ".png";

	if(!nameExists(GRAPHS_DIR))
		mkdir(GRAPHS_DIR, S_IRWXU | S_IRWXG | S_IRWXO);

	graphName = extractBasename(graphName);
	graphName.insert(0, GRAPHS_DIR);

	/* check for duplicates */
	int k = 2;
	while(nameExists(graphName)) {
		graphName = trimExtension(graphName);
		graphName += "_";
		size_t n = graphName.find_first_of("_");
		graphName.erase(n + 1);
		graphName += itos(k);
		graphName += ".png";
		++k;
	}

	string dot_cmd = "dot graph.dot -Tpng -o \"" + graphName;
	dot_cmd += "\"";
	ret = system(dot_cmd.c_str());
	if(!ret) {
		ret = unlink("graph.dot");
		if(ret)
			SHOWERR("deleting .dot file");
	}
	else
		SHOWERR("drawing graph");

	sample = NULL;
	SHOWMSG("analyzing finished");

	return true;
}
