/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#include "DatabaseOutput.h"
#include <core/Options.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/analyze/modules/ModuleManager.h>

#include <QtCore>
#include <QtSql>

DatabaseOutput::DatabaseOutput()
{
	m_name = "database";
    m_description = "Inserts info about samples into database.";
}

bool DatabaseOutput::exportOutput(ExploitSample *sample)
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
	if(exportGeneralData(info, sampleId) == false) {
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

bool DatabaseOutput::exportGeneralData(ExploitInfo *info, int id)
{
	LOG("exporting sample info [general] to database\n");
	QSqlQuery sampleQuery(DatabaseManager::instance()->database());
	sampleQuery.prepare("INSERT INTO analyze_sample VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sampleQuery.addBindValue(id);
	sampleQuery.addBindValue(info->name());
	sampleQuery.addBindValue(info->extractedFrom());
	sampleQuery.addBindValue(info->graphName());
	sampleQuery.addBindValue(info->captureDate().toString("yyyy-MM-dd"));
	sampleQuery.addBindValue(QString().setNum(info->size()));
	sampleQuery.addBindValue(info->fileType());
	sampleQuery.addBindValue(QString().setNum(info->fileSize()));
	sampleQuery.addBindValue(QString().setNum(info->codeOffset()));
	sampleQuery.addBindValue("");
	if(!DatabaseManager::instance()->exec(&sampleQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::exportAnalyzeData(ExploitInfo *info, int id)
{
	TraitsMap::iterator tableIt;
	TraitsEntry::iterator colIt;

	for(tableIt = info->traits()->begin(); tableIt != info->traits()->end(); ++tableIt) {
		QString tableName = ANALYZE_PREFIX + tableIt.key();

		/* get next sample_id number */
		int recordId = DatabaseManager::instance()->sequenceValue(ANALYZE_PREFIX + "_id_seq");

		/* prepare query */
		QString queryTemplate = "INSERT INTO ? VALUES (?";
		int columns = tableIt.value()->size();
		for(int i = 0; i < columns; ++i)
			queryTemplate += ", ?";
		queryTemplate += ")";

		QSqlQuery analyzeDataQuery(DatabaseManager::instance()->database());
		analyzeDataQuery.prepare(queryTemplate);
		analyzeDataQuery.addBindValue(tableName);
		analyzeDataQuery.addBindValue(recordId);

		for(colIt = tableIt.value()->begin(); colIt != tableIt.value()->end(); ++colIt)
			analyzeDataQuery.addBindValue(colIt.value());
	}

	LOG("SUCCESS\n\n");
	return true;
}
