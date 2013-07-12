/*
 *  Filename	: DatabaseInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 12-07-2013
 */

#include "DatabaseInput.h"

#include <list>
#include <string>
#include <sstream>
#include <QDate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <tasks/group/modules/input/IInput.h>
#include <tasks/group/GroupTask.h>
#include <tasks/analyze/modules/ModulesManager.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>
#include <core/ExploitSample.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>

using namespace std;
using namespace Group;

DatabaseInput::DatabaseInput()
{
	m_name = "DatabaseInput";
	m_type = "database";
	m_description = "Loads already analyzed exploits from database.";
}

bool DatabaseInput::loadInput(GroupTask *context, SampleList *samples)
{
	// select all samples
	stringstream ss;
	ss << "SELECT * FROM analyze_sample WHERE " << "capture_date >= '" << context->from().toString("yyyy-MM-dd").toStdString() << "' AND "
												<< "capture_date <= '" << context->until().toString("yyyy-MM-dd").toStdString() << "'";

	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare(ss.str().c_str());
	if(!DatabaseManager::instance()->exec(&selectQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	Analyze::AnalyzeMap *anaMods = Analyze::ModulesManager::instance()->analyze();
	Analyze::AnalyzeMap::iterator anaIt;
	ExploitSampleHandle sample;
	while(selectQuery.next()) {
		int sampleId = selectQuery.record().value("id").toInt();
		sample = ExploitSampleHandle(new ExploitSample());
		sample->info()->setName(selectQuery.record().value("name").toString().toStdString());
		sample->info()->setExtractedFrom(selectQuery.record().value("extracted_from").toString().toStdString());
		sample->info()->setGraphName(selectQuery.record().value("graph_name").toString().toStdString());
		sample->info()->setSize(selectQuery.record().value("size").toInt());
		sample->info()->setFileType(selectQuery.record().value("file_type").toString().toStdString());
		sample->info()->setFileSize(selectQuery.record().value("file_size").toInt());
		sample->info()->setCodeOffset(selectQuery.record().value("shellcode_offset").toInt());

		// if task files not empty, then check if sample matches criteria
		if(context->taskFiles().empty() == false) {
			bool pathFound = false;
			for(string path : context->taskFiles()) {
				if(sample->info()->extractedFrom().find(path) != string::npos) {
					pathFound = true;
					break;
				}
			}

			if(pathFound == false)
				continue;
		}

		// get analyze specific information about current sample
		bool status;
		for(anaIt = anaMods->begin(); anaIt != anaMods->end(); ++anaIt) {
			status = anaIt->second->importFromDatabase(sample, sampleId);
			if(status == false) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		samples->push_back(sample);
	}

	LOG("SUCCESS\n\n");
	return true;
}
