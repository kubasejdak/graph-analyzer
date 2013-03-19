/*
 * Filename		: LoopDetector.cpp
 * Author		: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#include "LoopDetector.h"
#include <core/Graph.h>
#include <utils/InstructionSplitter.h>
#include <utils/Toolbox.h>

LoopDetector::LoopDetector()
{
    m_name = "LoopDetector";
    m_description = "Detects loops and returns detailed information.";
    m_traitName = "loop";
}

bool LoopDetector::perform(ExploitSample *sample)
{
    Graph *g = sample->graph();
	Graph::graph_iterator it;
    LoopContainer *loops;
    LoopVec *vec;
	instr_vertex *iv;
	TraitsEntry *m;
	InstructionSplitter splitter;
    QString vertexes;
	for(it = g->begin(); it != g->end(); ++it) {
		loops = g->detectLoop(it);
		if(loops == NULL)
			continue;

		/* extract information about loops */
        LOG("loop found\n");
        for(int i = 0; i < loops->size(); ++i) {
			vec = (*loops)[i];
			iv = (instr_vertex *) vec->front()->data;
			m = new TraitsEntry();

			/* start address */
            (*m)["start"] = Toolbox::itos(iv->eip, 16);
            LOG("start: [%s]\n", Toolbox::itos(iv->eip, 16).toStdString().c_str());

			/* number of vertexes */
            (*m)["size"] = Toolbox::itos(vec->size());
            LOG("size: [%s]\n", Toolbox::itos(vec->size()).toStdString().c_str());

			/* list of vertexes */
			vertexes = "";
            for(int j = 0; j < vec->size(); ++j) {
				iv = (instr_vertex *) vec->at(j)->data;
                vertexes += Toolbox::itos(iv->eip, 16);
				if(j != vec->size() - 1)
					vertexes += ", ";
			}
			(*m)["vertexes"] = vertexes;
            LOG("vertexes: [%s]\n", vertexes.toStdString().c_str());

			/* number of iterations */
			emu_edge *e = emu_edges_first(vec->back()->edges);
			for(; !emu_edges_attail(e); e = emu_edges_next(e)) {
				iv = (instr_vertex *) e->destination->data;
                if(Toolbox::itos(iv->eip, 16) == (*m)["start"]) {
                    (*m)["iterations"] = Toolbox::itos(e->count);
                    LOG("iterations: [%s]\n", Toolbox::itos(e->count).toStdString().c_str());
					break;
				}

			}

			/* loop hash */
            QString loopString = "";
            for(int k = 0; k < vec->size(); ++k) {
				iv = (instr_vertex *) vec->at(i)->data;
				splitter = emu_string_char(iv->instr_string);
                loopString += splitter.code();
                loopString += splitter.firstArg();
                loopString += splitter.secondArg();
                loopString += splitter.syscall();
				splitter.clear();
			}

            (*m)["hash"] = Toolbox::hash(loopString);

			/* set traits */
            sample->info()->setTrait(m_traitName, m);
		}

		/* delete returned container */
        for(int i = 0; i < loops->size(); ++i)
			delete (*loops)[i];

		delete loops;
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool LoopDetector::exportToDatabase(ExploitSample *sample, int sampleId)
{
	/* get sample traits */
	TraitsMap *traits = sample->info()->traits();
	TraitsMap::iterator it;;

	/* for all api traits in sample*/
	for(it = traits->find(m_traitName); it != traits->end() && it.key() == m_traitName; ++it) {
		/* check if syscall/DLL is unique */
		QString start = it.value()->value("start");
		QString size = it.value()->value("size");
		QString vertexes = it.value()->value("vertexes");
		QString iterations = it.value()->value("iterations");
		QString hash = it.value()->value("hash");
		QSqlQuery selectQuery(DatabaseManager::instance()->database());
		selectQuery.prepare("SELECT * FROM analyze_loop WHERE hash = ?");
		selectQuery.addBindValue(hash);
		if(!DatabaseManager::instance()->exec(&selectQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		int loopId;
		/* if entry in database exists */
		if(selectQuery.next())
			loopId = selectQuery.record().value("id").toInt();
		else {
			/* get next api_id number */
			loopId = DatabaseManager::instance()->sequenceValue("analyze_loop_id_seq");

			/* insert data */
			QSqlQuery insertQuery(DatabaseManager::instance()->database());
			insertQuery.prepare("INSERT INTO analyze_loop VALUES (?, ?, ?, ?, ?, ?, ?)");
			insertQuery.addBindValue(loopId);
			insertQuery.addBindValue(hash);
			insertQuery.addBindValue(iterations);
			insertQuery.addBindValue(size);
			insertQuery.addBindValue(start);
			insertQuery.addBindValue(vertexes);
			insertQuery.addBindValue("");
			if(!DatabaseManager::instance()->exec(&insertQuery)) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		/* add assignment to sample */
		QSqlQuery insert2Query(DatabaseManager::instance()->database());
		insert2Query.prepare("INSERT INTO analyze_loopassignment VALUES (DEFAULT, ?, ?)");
		insert2Query.addBindValue(loopId);
		insert2Query.addBindValue(sampleId);
		if(!DatabaseManager::instance()->exec(&insert2Query)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
