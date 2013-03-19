/*
 *  Filename	: SyscallAnalyze.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#include "SyscallAnalyze.h"
#include <core/Graph.h>
#include <utils/InstructionSplitter.h>

SyscallAnalyze::SyscallAnalyze()
{
    m_name = "SyscallAnalyze";
    m_description = "Searches the graph for system API calls.";
    m_traitName = "api";
}

bool SyscallAnalyze::perform(ExploitSample *sample)
{
    Graph *g = sample->graph();
	struct instr_vertex *instr_vert;
    QString syscall, dll;
	Graph::graph_iterator it;
	InstructionSplitter splitter;
	TraitsEntry *m;

	for(it = g->begin(); it != g->end(); ++it) {
		instr_vert = (struct instr_vertex *) it->data;
		if(instr_vert->dll) {
			splitter = emu_string_char(instr_vert->instr_string);
			dll = instr_vert->dll->dllname;
			dll += ".dll";
            syscall = splitter.syscall();
			m = new TraitsEntry();
			(*m)["syscall"] = syscall;
            LOG("syscall: [%s]\n", syscall.toStdString().c_str());
			(*m)["DLL"] = dll;
            LOG("DLL: [%s]\n", dll.toStdString().c_str());
            sample->info()->setTrait(m_traitName, m);
		}
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool SyscallAnalyze::exportToDatabase(ExploitSample *sample, int sampleId)
{
	/* get sample traits */
	TraitsMap *traits = sample->info()->traits();
	TraitsMap::iterator it;

	/* for all api traits in sample*/
	for(it = traits->find(m_traitName); it != traits->end() && it.key() == m_traitName; ++it) {
		/* check if syscall/DLL is unique */
		QString syscall = it.value()->value("syscall");
		QString dll = it.value()->value("DLL");
		QSqlQuery selectQuery(DatabaseManager::instance()->database());
		selectQuery.prepare("SELECT * FROM analyze_api WHERE syscall = ? AND dll = ?");
		selectQuery.addBindValue(syscall);
		selectQuery.addBindValue(dll);
		if(!DatabaseManager::instance()->exec(&selectQuery))
			return false;

		int apiId;
		/* if entry in database exists */
		if(selectQuery.next())
			apiId = selectQuery.record().value("id").toInt();
		else {
			/* get next api_id number */
			apiId = DatabaseManager::instance()->sequenceValue("analyze_api_id_seq");

			/* insert data */
			QSqlQuery insertQuery(DatabaseManager::instance()->database());
			insertQuery.prepare("INSERT INTO analyze_api VALUES (?, ?, ?)");
			insertQuery.addBindValue(apiId);
			insertQuery.addBindValue(dll);
			insertQuery.addBindValue(syscall);
			if(!DatabaseManager::instance()->exec(&insertQuery))
				return false;
		}

		/* add assignment to sample */
		QSqlQuery insert2Query(DatabaseManager::instance()->database());
		insert2Query.prepare("INSERT INTO analyze_apiassignment VALUES (DEFAULT, ?, ?)");
		insert2Query.addBindValue(apiId);
		insert2Query.addBindValue(sampleId);
		if(!DatabaseManager::instance()->exec(&insert2Query))
			return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}
