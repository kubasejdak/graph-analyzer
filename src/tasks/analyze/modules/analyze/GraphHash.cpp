/*
 * Filename		: GraphHash.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#include "GraphHash.h"
#include <core/Graph.h>
#include <utils/InstructionSplitter.h>
#include <utils/Toolbox.h>

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
    QString graphString = "";
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
    LOG("hash: [%s]\n", Toolbox::hash(graphString).toStdString().c_str());

	/* set traits */
    sample->info()->setTrait(m_traitName, m);

    LOG("SUCCESS\n\n");
	return true;
}

bool GraphHash::exportToDatabase(ExploitSample *sample, int sampleId)
{
	/* get sample traits */
	TraitsMap *traits = sample->info()->traits();
	TraitsMap::iterator it;

	/* for all api traits in sample*/
	for(it = traits->find(m_traitName); it != traits->end() && it.key() == m_traitName; ++it) {
		/* check if syscall/DLL is unique */
		QString hash = it.value()->value("hash");
		QSqlQuery selectQuery(DatabaseManager::instance()->database());
		selectQuery.prepare("SELECT * FROM analyze_hash WHERE hash = ?");
		selectQuery.addBindValue(hash);
		if(!DatabaseManager::instance()->exec(&selectQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		int hashId;
		/* if entry in database exists */
		if(selectQuery.next())
			hashId = selectQuery.record().value("id").toInt();
		else {
			/* get next api_id number */
			hashId = DatabaseManager::instance()->sequenceValue("analyze_hash_id_seq");

			/* insert data */
			QSqlQuery insertQuery(DatabaseManager::instance()->database());
			insertQuery.prepare("INSERT INTO analyze_hash VALUES (?, ?)");
			insertQuery.addBindValue(hashId);
			insertQuery.addBindValue(hash);
			if(!DatabaseManager::instance()->exec(&insertQuery)) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		/* add assignment to sample */
		QSqlQuery insert2Query(DatabaseManager::instance()->database());
		insert2Query.prepare("INSERT INTO analyze_hashassignment VALUES (DEFAULT, ?, ?)");
		insert2Query.addBindValue(hashId);
		insert2Query.addBindValue(sampleId);
		if(!DatabaseManager::instance()->exec(&insert2Query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
