/*
 *  Filename	: SyscallAnalyze.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#include "SyscallAnalyze.h"

#include <string>
#include <sstream>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <core/Graph.h>
#include <core/ExploitSample.h>
#include <utils/InstructionSplitter.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>

using namespace std;

SyscallAnalyze::SyscallAnalyze()
{
    m_name = "SyscallAnalyze";
    m_description = "Searches the graph for system API calls.";
    m_traitName = "api";
}

bool SyscallAnalyze::perform(ExploitSampleHandle sample)
{
    GraphHandle g = sample->graph();
	struct instr_vertex *instr_vert;
	string syscall, dll;
	Graph::graph_iterator it;
	InstructionSplitter splitter;

	for(it = g->begin(); it != g->end(); ++it) {
		instr_vert = (struct instr_vertex *) it->data;
		if(instr_vert->dll) {
			splitter = emu_string_char(instr_vert->instr_string);
			dll = instr_vert->dll->dllname;
			dll += ".dll";
            syscall = splitter.syscall();
			TraitEntryHandle m(new TraitEntry());
			(*m)["syscall"] = syscall;
			LOG("syscall: [%s]\n", syscall.c_str());
			(*m)["DLL"] = dll;
			LOG("DLL: [%s]\n", dll.c_str());
            sample->info()->setTrait(m_traitName, m);
		}
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool SyscallAnalyze::exportToDatabase(ExploitSampleHandle sample, int sampleId)
{
	// get sample traits
	TraitMapHandle traits = sample->info()->traits();
	TraitMap::iterator it;

	// for all api traits in sample
	for(it = traits->find(m_traitName); it != traits->end() && it.key() == m_traitName; ++it) {
		// check if syscall/DLL is unique
		string syscall = it.value()->value("syscall");
		string dll = it.value()->value("DLL");

		stringstream ss;
		ss << "SELECT * FROM analyze_api WHERE syscall = '" << syscall << "' AND dll = '" << dll << "'";

		QSqlQuery selectQuery(DatabaseManager::instance()->database());
		selectQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&selectQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		int apiId;
		// if entry in database exists
		if(selectQuery.next())
			apiId = selectQuery.record().value("id").toInt();
		else {
			// get next api_id number
			apiId = DatabaseManager::instance()->sequenceValue("analyze_api_id_seq");

			// insert data
			stringstream ss;
			ss << "INSERT INTO analyze_api VALUES (" << apiId << ", '" << dll << "', '" << syscall << "')";

			QSqlQuery insertQuery(DatabaseManager::instance()->database());
			insertQuery.prepare(ss.str().c_str());
			if(!DatabaseManager::instance()->exec(&insertQuery)) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		// add assignment to sample
		ss.str("");
		ss << "INSERT INTO analyze_apiassignment VALUES (DEFAULT, " << apiId << ", " << sampleId << ")";

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

bool SyscallAnalyze::importFromDatabase(ExploitSampleHandle sample, int sampleId)
{
	stringstream ss;
	ss << "SELECT * FROM analyze_apiassignment WHERE sample_id = " << sampleId;

	QSqlQuery selectSyscallIdQuery(DatabaseManager::instance()->database());
	selectSyscallIdQuery.prepare(ss.str().c_str());
	if(!DatabaseManager::instance()->exec(&selectSyscallIdQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	while(selectSyscallIdQuery.next()) {
		int apiId = selectSyscallIdQuery.record().value("api_id").toInt();

		ss.str("");
		ss << "SELECT * FROM analyze_api WHERE id = " << apiId;

		QSqlQuery selectSyscallQuery(DatabaseManager::instance()->database());
		selectSyscallQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&selectSyscallQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		if(selectSyscallQuery.next() == false) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		TraitEntryHandle m = TraitEntryHandle(new TraitEntry());
		(*m)["syscall"] = selectSyscallQuery.record().value("syscall").toString().toStdString();
		(*m)["DLL"] = selectSyscallQuery.record().value("dll").toString().toStdString();

		// set traits
		sample->info()->setTrait(m_traitName, m);
	}

	LOG("SUCCESS\n\n");
	return true;
}
