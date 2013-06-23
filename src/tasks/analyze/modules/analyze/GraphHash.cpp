/*
 * Filename		: GraphHash.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#include "GraphHash.h"

#include <string>
#include <sstream>
#include <QtSql>

#include <core/Graph.h>
#include <core/ExploitSample.h>
#include <utils/InstructionSplitter.h>
#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>

using namespace std;

GraphHash::GraphHash()
{
    m_name = "GraphHash";
    m_description = "Hashes whole graph for further comparison.";
    m_traitName = "hash";
}

bool GraphHash::perform(ExploitSample *sample)
{
    Graph *g = sample->graph();
	Graph::graph_iterator it;
	TraitsEntry *m = new TraitsEntry();
	string graphString = "";
	InstructionSplitter splitter;
	instr_vertex *iv;
	for(it = g->begin(); it != g->end(); ++it) {
		iv = (instr_vertex *) it->data;
		splitter = emu_string_char(iv->instr_string);
        graphString += splitter.code();
        graphString += splitter.firstArg();
        graphString += splitter.secondArg();
        graphString += splitter.syscall();
		splitter.clear();
	}

    (*m)["hash"] = Toolbox::hash(graphString);
	LOG("hash: [%s]\n", Toolbox::hash(graphString).c_str());

	// set traits
    sample->info()->setTrait(m_traitName, m);

    LOG("SUCCESS\n\n");
	return true;
}

bool GraphHash::exportToDatabase(ExploitSample *sample, int sampleId)
{
	// get sample traits
	TraitsMap *traits = sample->info()->traits();
	TraitsMap::iterator it;

	// for all api traits in sample
	for(it = traits->find(m_traitName); it != traits->end() && it.key() == m_traitName; ++it) {
		// check if syscall/DLL is unique
		string hash = it.value()->value("hash");

		stringstream ss;
		ss << "SELECT * FROM analyze_hash WHERE hash = '" << hash << "'";

		QSqlQuery selectQuery(DatabaseManager::instance()->database());
		selectQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&selectQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		int hashId;
		// if entry in database exists
		if(selectQuery.next())
			hashId = selectQuery.record().value("id").toInt();
		else {
			// get next api_id number
			hashId = DatabaseManager::instance()->sequenceValue("analyze_hash_id_seq");

			// insert data
			stringstream ss;
			ss << "INSERT INTO analyze_hash VALUES (" << hashId << ", '" << hash << "')";

			QSqlQuery insertQuery(DatabaseManager::instance()->database());
			insertQuery.prepare(ss.str().c_str());
			if(!DatabaseManager::instance()->exec(&insertQuery)) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		// add assignment to sample
		ss.str("");
		ss << "INSERT INTO analyze_hashassignment VALUES (DEFAULT, " << hashId << ", " << sampleId << ")";

		QSqlQuery insert2Query(DatabaseManager::instance()->database());
		insert2Query.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&insert2Query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
