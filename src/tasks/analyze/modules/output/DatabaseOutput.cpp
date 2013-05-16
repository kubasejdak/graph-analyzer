/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#include "DatabaseOutput.h"
#include <core/Options.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <utils/Toolbox.h>
#include <tasks/analyze/modules/ModuleManager.h>

#include <QtCore>
#include <QtSql>

DatabaseOutput::DatabaseOutput()
{
	m_name = "database";
    m_description = "Inserts info about samples into database.";
}

bool DatabaseOutput::exportOutput(ExploitSample *sample, int taskId)
{
    ExploitInfo *info = sample->info();

    /* ensure that sample is not a duplicate */
    bool duplicate = checkDuplicate(info);
    if(duplicate) {
        LOG("duplicate sample: skipping and removing duplicated graph file\n");
		QFile(info->graphName()).remove();
        LOG("SUCCESS\n\n");
        return true;
    }

	/* get next sample_id number */
	int sampleId = DatabaseManager::instance()->sequenceValue("analyze_sample_id_seq");

	/* general sample data */
    if(exportGeneralData(info, sampleId, taskId) == false) {
        LOG_ERROR("exporting general sample info to database [%s]\n", info->name().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

    /* analyze modules specific data */
	AnalyzeMap *anaMods = ModuleManager::instance()->analyze();
	AnalyzeMap::iterator anaIt;
	for(anaIt = anaMods->begin(); anaIt != anaMods->end(); ++anaIt) {
		LOG("exporting sample info [%s] to database\n", anaIt.key().toStdString().c_str());
		anaIt.value()->exportToDatabase(sample, sampleId);
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::checkDuplicate(ExploitInfo *info)
{
	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare("SELECT * FROM analyze_sample WHERE name = ? AND extracted_from = ? AND file_size = ? AND shellcode_offset = ?");
	selectQuery.addBindValue(info->name());
	selectQuery.addBindValue(info->extractedFrom());
	selectQuery.addBindValue(info->size());
	selectQuery.addBindValue(info->codeOffset());
	if(!DatabaseManager::instance()->exec(&selectQuery)) {
		LOG_ERROR("FAILURE\n\n");
        return false;
	}

    LOG("SUCCESS\n\n");
	return selectQuery.next();
}

bool DatabaseOutput::exportGeneralData(ExploitInfo *info, int sampleId, int taskId)
{
	QString d = info->captureDate().toString("yyyy-MM-dd");
	QString captureDate = (d == "1999-12-31") ? "n/a" : d;

	LOG("exporting sample info [general] to database\n");
	QSqlQuery sampleQuery(DatabaseManager::instance()->database());
    sampleQuery.prepare("INSERT INTO analyze_sample VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sampleQuery.addBindValue(sampleId);
	sampleQuery.addBindValue(info->name());
	sampleQuery.addBindValue(info->extractedFrom());
	sampleQuery.addBindValue(info->graphName());
	sampleQuery.addBindValue(captureDate);
	sampleQuery.addBindValue(Toolbox::itos(info->size()));
	sampleQuery.addBindValue(info->fileType());
	sampleQuery.addBindValue(Toolbox::itos(info->fileSize()));
	sampleQuery.addBindValue(Toolbox::itos(info->codeOffset()));
	sampleQuery.addBindValue("");
    sampleQuery.addBindValue(taskId);
	if(!DatabaseManager::instance()->exec(&sampleQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}
