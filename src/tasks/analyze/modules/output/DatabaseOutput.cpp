/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#include "DatabaseOutput.h"

#include <string>
#include <sstream>
#include <QSqlQuery>
#include <QFile>

#include <core/Options.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <utils/Toolbox.h>
#include <tasks/analyze/modules/ModuleManager.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>
#include <tasks/analyze/modules/output/IOutput.h>

using namespace std;
using namespace Analyze;

DatabaseOutput::DatabaseOutput()
{
	m_name = "database";
    m_description = "Inserts info about samples into database.";
}

bool DatabaseOutput::exportOutput(ExploitSampleHandle sample, int taskId, bool override)
{
    ExploitInfoHandle info = sample->info();

	// ensure that sample is not a duplicate
    bool duplicate = checkDuplicate(info);
	if(duplicate && override == false) {
        LOG("duplicate sample: skipping and removing duplicated graph file\n");
		QFile(info->graphName().c_str()).remove();
        LOG("SUCCESS\n\n");
        return true;
    }

	// get next sample_id number
	int sampleId = DatabaseManager::instance()->sequenceValue("analyze_sample_id_seq");

	// general sample data
    if(exportGeneralData(info, sampleId, taskId) == false) {
		LOG_ERROR("exporting general sample info to database [%s]\n", info->name().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	// analyze modules specific data
	AnalyzeMap *anaMods = ModuleManager::instance()->analyze();
	AnalyzeMap::iterator anaIt;
	for(anaIt = anaMods->begin(); anaIt != anaMods->end(); ++anaIt) {
		LOG("exporting sample info [%s] to database\n", anaIt->first.c_str());
		anaIt->second->exportToDatabase(sample, sampleId);
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::checkDuplicate(ExploitInfoHandle info)
{
	stringstream ss;
	ss << "SELECT * FROM analyze_sample WHERE " << "name = '" << info->name() << "'"
											   << " AND extracted_from = '" << info->extractedFrom() << "'"
											   << " AND file_size = '" << info->size()  << "'"
											   << " AND shellcode_offset = '" << info->codeOffset() << "'";

	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare(ss.str().c_str());
	if(!DatabaseManager::instance()->exec(&selectQuery)) {
		LOG_ERROR("FAILURE\n\n");
        return false;
	}

    LOG("SUCCESS\n\n");
	return selectQuery.next();
}

bool DatabaseOutput::exportGeneralData(ExploitInfoHandle info, int sampleId, int taskId)
{
	string d = info->captureDate().toString("yyyy-MM-dd").toStdString();
	string captureDate = (d == "1999-12-31") ? "n/a" : d;

	LOG("exporting sample info [general] to database\n");
	stringstream ss;
	ss << "INSERT INTO analyze_sample VALUES (" << sampleId
												<< ", '" << info->name() << "'"
												<< ", '" << info->extractedFrom() << "'"
												<< ", '" << info->graphName() << "'"
												<< ", '" << captureDate << "'"
												<< ", '" << Toolbox::itos(info->size()) << "'"
												<< ", '" << info->fileType() << "'"
												<< ", '" << Toolbox::itos(info->fileSize()) << "'"
												<< ", '" << Toolbox::itos(info->codeOffset()) << "'"
												<< ", " << "''"
												<< ", " << taskId << ")";

	QSqlQuery sampleQuery(DatabaseManager::instance()->database());
	sampleQuery.prepare(ss.str().c_str());
	if(!DatabaseManager::instance()->exec(&sampleQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}
