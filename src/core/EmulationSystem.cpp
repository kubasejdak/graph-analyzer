/*
 *  Filename	: EmulationSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#include "EmulationSystem.h"

EmulationSystem::EmulationSystem()
{
	emuUnit = new EmulationUnit();
	sample = NULL;
}

EmulationSystem::~EmulationSystem()
{
	delete emuUnit;
}

void EmulationSystem::loadSample(ShellcodeSample *sample)
{
	this->sample = sample;
}

bool EmulationSystem::emulate()
{
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
		ehi = emu_hashtable_search(eh, (void *) (uintptr_t) eipsave);
		if(ehi != NULL)
			ev = (struct emu_vertex *) ehi->value;

		/* create new vertex if eip is unique */
		if(ev == NULL) {
			ev = emu_vertex_new();
			emu_graph_vertex_add(eg, ev);
			emu_hashtable_insert(eh, (void *) (uintptr_t) eipsave, ev);
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
				if(eipsave > env->env.win->loaded_dlls[numdlls]->baseaddr
						&& eipsave < env->env.win->loaded_dlls[numdlls]->baseaddr
						+ env->env.win->loaded_dlls[numdlls]->imagesize) {
					iv->dll = env->env.win->loaded_dlls[numdlls];
				}
				++numdlls;
			}

			/* no function defined fot this name */
			if(hook->hook.win->fnhook == NULL) {
				LOG("unhooked call to %s\n", hook->hook.win->fnname);
				return false;
			}
		}
		/* if not in any dll */
		else {
			ret = emu_cpu_parse(cpu);

			struct emu_env_hook *hook = NULL;
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
					LOG("cpu %s\n", emu_strerror(emuUnit->getEmu()));
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
			if(cpu->instr.is_fpu == 0 && cpu->instr.source.cond_pos == eipsave
					&& cpu->instr.source.has_cond_pos == 1)
				ee->data = (void *) 0x1;
		}
		last_vertex = ev;
	} /* emulation loop */

	/* create .dot file */
	string dot_file = APP_ROOT_PATH;
	dot_file.append("bin/graph.dot");
	graph_draw(graph->getEmuGraph(), dot_file);

	/* draw graph using dot package and sample name */
	string graph_name = sample->getInfo()->getName();
	graph_name = trimExtension(graph_name);
	graph_name += ".png";

	if(!nameExists(GRAPHS_DIR)) {
		int ret = mkdir(GRAPHS_DIR.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
		if(ret != 0) {
			LOG_ERROR("cannot create directory %s\n", GRAPHS_DIR.c_str());
			return false;
		}
	}

	graph_name = extractBasename(graph_name);
	graph_name.insert(0, GRAPHS_DIR);

	/* check for duplicates */
	int k = 2;
	while(nameExists(graph_name)) {
		graph_name = trimExtension(graph_name);
		graph_name += "_";
		size_t n = graph_name.find_first_of("_");
		graph_name.erase(n + 1);
		graph_name += itos(k);
		graph_name += ".png";
		++k;
	}

	string dot_cmd = "dot ";
	dot_cmd.append(dot_file.c_str()).append(" -Tpng -o \"");
	dot_cmd.append(graph_name.c_str()).append("\"");
	ret = system(dot_cmd.c_str());
	if(!ret) {
		ret = unlink(dot_file.c_str());
		if(ret) {
			SystemLogger::getInstance()->setError(UNLINK_FAILED);
			LOG_ERROR("deleting .dot file\n");
		}
	}
	else {
		SystemLogger::getInstance()->setError(GRAPH_DRAW_FAILED);
		LOG_ERROR("drawing graph failed\n");
		ret = unlink(dot_file.c_str());
		if(ret)
			LOG_ERROR("deleting .dot file\n");
	}

	if(DELETE_CODE_INSTANTLY) {
		delete sample->getCode();
		sample->setCode(NULL);
	}

	sample->getInfo()->setGraphName(graph_name);
	sample = NULL;
	LOG("emulation finished\n");

	return true;
}
