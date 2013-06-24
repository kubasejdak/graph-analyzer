/*
 * Filename		: LoopDetector.cpp
 * Author		: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#include "LoopDetector.h"

#include <string>
#include <sstream>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <core/Graph.h>
#include <core/ExploitSample.h>
#include <utils/InstructionSplitter.h>
#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>

using namespace std;

LoopDetector::LoopDetector()
{
    m_name = "LoopDetector";
    m_description = "Detects loops and returns detailed information.";
    m_traitName = "loop";
}

bool LoopDetector::perform(ExploitSampleHandle sample)
{
    GraphHandle g = sample->graph();
	Graph::graph_iterator it;
    LoopVector *loops;
    VertexVector *vec;
	instr_vertex *iv;
	TraitEntryHandle m;
	InstructionSplitter splitter;
	string vertexes;

	for(it = g->begin(); it != g->end(); ++it) {
		loops = g->detectLoop(it);
		if(loops == NULL || loops->size() == 0)
			continue;

		// extract information about loops
        LOG("loop found\n");
		for(unsigned int i = 0; i < loops->size(); ++i) {
			vec = (*loops)[i];
			iv = (instr_vertex *) vec->front()->data;
			TraitEntryHandle m(new TraitEntry());

			// start address
            (*m)["start"] = Toolbox::itos(iv->eip, 16);
			LOG("start: [%s]\n", Toolbox::itos(iv->eip, 16).c_str());

			// number of vertexes
            (*m)["size"] = Toolbox::itos(vec->size());
			LOG("size: [%s]\n", Toolbox::itos(vec->size()).c_str());

			// list of vertexes
			vertexes = "";
			for(unsigned int j = 0; j < vec->size(); ++j) {
				iv = (instr_vertex *) vec->at(j)->data;
                vertexes += Toolbox::itos(iv->eip, 16);
				if(j != vec->size() - 1)
					vertexes += ", ";
			}
			(*m)["vertexes"] = vertexes;
			LOG("vertexes: [%s]\n", vertexes.c_str());

			// number of iterations
			emu_edge *e = emu_edges_first(vec->back()->edges);
			for(; !emu_edges_attail(e); e = emu_edges_next(e)) {
				iv = (instr_vertex *) e->destination->data;
                if(Toolbox::itos(iv->eip, 16) == (*m)["start"]) {
                    (*m)["iterations"] = Toolbox::itos(e->count);
					LOG("iterations: [%s]\n", Toolbox::itos(e->count).c_str());
					break;
				}

			}

			// loop hash
			string loopString = "";
			for(unsigned int k = 0; k < vec->size(); ++k) {
				iv = (instr_vertex *) vec->at(i)->data;
				splitter = emu_string_char(iv->instr_string);
                loopString += splitter.code();
                loopString += splitter.firstArg();
                loopString += splitter.secondArg();
                loopString += splitter.syscall();
				splitter.clear();
			}

            (*m)["hash"] = Toolbox::hash(loopString);

			// set traits
            sample->info()->setTrait(m_traitName, m);
		}

		// delete returned container
		for(unsigned int i = 0; i < loops->size(); ++i)
			delete (*loops)[i];

		delete loops;
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool LoopDetector::exportToDatabase(ExploitSampleHandle sample, int sampleId)
{
	// get sample traits
	TraitMapHandle traits = sample->info()->traits();
	TraitMap::iterator it;;

	// for all api traits in sample
	for(it = traits->find(m_traitName); it != traits->end() && it.key() == m_traitName; ++it) {
		// check if syscall/DLL is unique
		string start = it.value()->value("start");
		string size = it.value()->value("size");
		string vertexes = it.value()->value("vertexes");
		string iterations = it.value()->value("iterations");
		string hash = it.value()->value("hash");

		stringstream ss;
		ss << "SELECT * FROM analyze_loop WHERE hash = '" << hash << "'";

		QSqlQuery selectQuery(DatabaseManager::instance()->database());
		selectQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&selectQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		int loopId;
		// if entry in database exists
		if(selectQuery.next())
			loopId = selectQuery.record().value("id").toInt();
		else {
			// get next api_id number
			loopId = DatabaseManager::instance()->sequenceValue("analyze_loop_id_seq");

			// insert data
			stringstream ss;
			ss << "INSERT INTO analyze_loop VALUES (" << loopId << ", "
													  << "'" << hash << "', "
													  << "'" << iterations << "', "
													  << "'" << size << "', "
													  << "'" << start << "', "
													  << "'" << vertexes << "', "
													  << "'')";

			QSqlQuery insertQuery(DatabaseManager::instance()->database());
			insertQuery.prepare(ss.str().c_str());
			if(!DatabaseManager::instance()->exec(&insertQuery)) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		// add assignment to sample
		ss.str("");
		ss << "INSERT INTO analyze_loopassignment VALUES (DEFAULT, " << loopId << ", " << sampleId << ")";

		QSqlQuery insert2Query(DatabaseManager::instance()->database());
		insert2Query.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&insert2Query)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
