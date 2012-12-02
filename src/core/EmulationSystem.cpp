/*
 *  Filename	: EmulationSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#include "EmulationSystem.h"

EmulationSystem::EmulationSystem()
{
    m_emuUnit = new EmulationUnit();
    m_sample = NULL;
}

EmulationSystem::~EmulationSystem()
{
    delete m_emuUnit;
}

void EmulationSystem::loadSample(ShellcodeSample *sample)
{
    m_sample = sample;
}

bool EmulationSystem::emulate()
{
    if(!m_sample) {
        LOG_ERROR("m_sample: [null]\n");
        LOG_ERROR("FAILURE\n\n");
		return false;
    }

	/* load code to emu unit */
	int32_t codeOffset;
	codeOffset = m_emuUnit->loadCode(m_sample->code(), m_sample->info()->fileSize());
    m_sample->info()->setCodeOffset(codeOffset);
    m_sample->info()->setShellcodePresent(codeOffset >= 0 ? true : false);

	/* if exploit not detected return */
    if(!m_sample->info()->isShellcodePresent()) {
        LOG("exploit not detected, returning\n");
        LOG("SUCCESS\n\n");
        m_sample = NULL;
		return true;
	}

	/* prepare environment */
	uint32_t eipsave = 0;
	int ret = 0;

    Graph *graph = m_sample->graph();
    struct emu_cpu *cpu = m_emuUnit->cpu();
    struct emu_env *env = m_emuUnit->env();
    struct emu_graph *eg = graph->emuGraph();
    struct emu_hashtable *eh = graph->emuHashtable();
	struct emu_vertex *last_vertex = NULL;
	struct emu_vertex *ev = NULL;
	struct emu_hashtable_item *ehi = NULL;
	struct emu_env_hook *hook = NULL;
	struct instr_vertex *iv = NULL;

	/* turn on disassembling */
	emu_cpu_debugflag_set(cpu, instruction_string);

	/* emulation loop */
    LOG("entering emulation loop\n");
	for(int i = 0; i < Options::instance()->EMULATION_STEPS; ++i) {
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
                LOG_ERROR("unhooked call to: [%s]\n", hook->hook.win->fnname);
                LOG_ERROR("FAILURE\n\n");
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
                    else {
                        LOG_ERROR("hook.lin->fnhook: [null]\n");
                        LOG_ERROR("FAILURE\n\n");
						return false;
                    }
				}
			}

			if(ret == -1) {
				/* step failed - maybe SEH */
				if(emu_env_w32_step_failed(env) != 0) {
                    LOG("cpu: [%s]\n", emu_strerror(m_emuUnit->emu()));
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

	/* get number of vertexes in sample */
	m_sample->info()->setSize(m_sample->graph()->size());
	LOG("vertexes inside sample: [%d]\n", m_sample->info()->size());

	/* check if sample is broken */
	if(m_sample->info()->size() < Options::instance()->BROKEN_SAMPLE_SIZE) {
		m_sample->info()->setBroken(true);
		LOG("sample is broken, skipping graph drawing\n");
		LOG("emulation finished\n");
		LOG("SUCCESS\n\n");
		return true;
	}

	/* create .dot file */
    QString dotFile = "/tmp/graph.dot";
    LOG("dotFile: [%s]\n", dotFile.toStdString().c_str());
    graph_draw(graph->emuGraph(), dotFile);

	/* draw graph using dot package and sample name */
	if(!QDir(Options::instance()->GRAPHS_DIR).exists()) {
		bool success = QDir().mkdir(Options::instance()->GRAPHS_DIR);
        if(!success) {
			LOG_ERROR("cannot create directory: [%s]\n", Options::instance()->GRAPHS_DIR.toStdString().c_str());
            LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}
	LOG("using graphs directory: [%s]\n", Options::instance()->GRAPHS_DIR.toStdString().c_str());

    QFileInfo graphFile(m_sample->info()->name());
    QString graphName;
    if(m_sample->info()->fileType() == "pcap")
		graphName = QString("%1/%2.png").arg(Options::instance()->GRAPHS_DIR).arg(Toolbox::pcapFlowBasename(m_sample->info()->name()));
    else
		graphName = QString("%1/%2.png").arg(Options::instance()->GRAPHS_DIR).arg(graphFile.baseName());
    LOG("graphName: [%s]\n", graphName.toStdString().c_str());

	/* check for duplicates */
	int k = 2;
    while(QFile(graphName).exists()) {
        LOG("duplicate for graphName found, renaming\n");
        QFileInfo duplicate(graphName);
        graphName = QString("%1/%2").arg(duplicate.absolutePath()).arg(duplicate.baseName());

        if(k != 2) {
            int n = graphName.lastIndexOf('_');
            graphName.truncate(n + 1);
        }
        else
            graphName += "_";
        QString str_num;
        graphName += str_num.setNum(k) + ".png";
        LOG("renamed to graphName: [%s]\n", graphName.toStdString().c_str());
		++k;
	}
    LOG("final graphName: [%s]\n", graphName.toStdString().c_str());

    QString dotCmd = QString("dot %1 -Tpng -o \"%2\"").arg(dotFile.toStdString().c_str()).arg(graphName.toStdString().c_str());
    LOG("executing [%s]\n", dotCmd.toStdString().c_str());
    ret = system(dotCmd.toStdString().c_str());
	if(!ret) {
        bool success = QFile(dotFile).remove();
        if(!success) {
			LOG_ERROR("deleting .dot file\n");
            LOG_ERROR("FAILURE\n\n");
            return false;
        }
	}
	else {
		LOG_ERROR("drawing graph failed\n");
        bool success = QFile(dotFile).remove();
        if(!success) {
			LOG_ERROR("deleting .dot file\n");
            LOG_ERROR("FAILURE\n\n");
            return false;
        }
	}

	/* delete code to release memory */
	delete m_sample->code();
	m_sample->setCode(NULL);

    m_sample->info()->setGraphName(graphName);
    m_sample = NULL;
    LOG("emulation finished\n");
    LOG("SUCCESS\n\n");
	return true;
}
