/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"
#include <core/version.h>
#include <utils/SystemLogger.h>
#include <tasks/TaskManager.h>

CoreSystem::CoreSystem()
{
    SystemLogger::instance()->setStatus("idle");
    SystemLogger::instance()->clearError();

    LOG("created CoreSystem instance, version: [%s]\n\n", VERSION);
}

bool CoreSystem::executeTasks()
{
	TaskManager taskManager;

	if(!taskManager.collectTasks()) {
		LOG_ERROR("collecting tasks failed\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	if(!taskManager.executeTasks()) {
		LOG_ERROR("executing tasks failed\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

QString CoreSystem::status()
{
	return SystemLogger::instance()->status();
}

QString CoreSystem::lastError()
{
	return SystemLogger::instance()->error();
}

QString CoreSystem::version()
{
	return VERSION;
}

#if 0
bool CoreSystem::dbUpdateSystemInfo()
{
	/* check for info */
	QSqlQuery checkQuery(DatabaseManager::instance()->database());
	checkQuery.prepare("SELECT * FROM options_systeminfo");
	if(!DatabaseManager::instance()->exec(&checkQuery))
		return false;

	/* there is system info record in database */
	if(checkQuery.next()) {
		int id = checkQuery.record().value("id").toInt();

		QSqlQuery updateQuery(DatabaseManager::instance()->database());
		updateQuery.prepare("UPDATE options_systeminfo SET version = ?, status = ?, error = ?, progress = ?, exploits_num = ?, samples_num = ?, files_num = ?, errors_num = ? WHERE id = ?");
		updateQuery.addBindValue(version());
		updateQuery.addBindValue(status());
		updateQuery.addBindValue(lastError());
		updateQuery.addBindValue(progress());
		updateQuery.addBindValue(exploitsNum());
		updateQuery.addBindValue(samplesNum());
		updateQuery.addBindValue(filesNum());
		updateQuery.addBindValue(errorsNum());
		updateQuery.addBindValue(id);
		if(!DatabaseManager::instance()->exec(&updateQuery))
			return false;
	}
	/* there is no system info in database */
	else {
		int id = DatabaseManager::instance()->sequenceValue("options_systeminfo_id_seq");

		QSqlQuery insertQuery(DatabaseManager::instance()->database());
		insertQuery.prepare("INSERT INTO options_systeminfo VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
		insertQuery.addBindValue(id);
		insertQuery.addBindValue(version());
		insertQuery.addBindValue(status());
		insertQuery.addBindValue(error());
		insertQuery.addBindValue(progress());
		insertQuery.addBindValue(exploitsNum());
		insertQuery.addBindValue(samplesNum());
		insertQuery.addBindValue(filesNum());
		insertQuery.addBindValue(errorsNum());
		if(!DatabaseManager::instance()->exec(&insertQuery))
			return false;
	}

	return true;
}
#endif
