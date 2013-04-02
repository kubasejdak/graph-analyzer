/*
 *  Filename	: StatusExportStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 02-04-2013
 */

#include "StatusExportStrategy.h"
#include <utils/DatabaseManager.h>
#include <utils/SystemLogger.h>
#include <core/version.h>

void DBStatusExportStrategy::exportStatus(int progress)
{
    DatabaseManager::instance()->clearTable("options_systeminfo");
    int statusId = DatabaseManager::instance()->sequenceValue("options_systeminfo_id_seq");

    QSqlQuery statusQuery(DatabaseManager::instance()->database());
    statusQuery.prepare("INSERT INTO options_systeminfo VALUES (?, ?, ?, ?, ?, ?)");
    statusQuery.addBindValue(statusId);
    statusQuery.addBindValue(VERSION);
    statusQuery.addBindValue(SystemLogger::instance()->status());
    statusQuery.addBindValue(progress);
    statusQuery.addBindValue(SystemLogger::instance()->error());
    statusQuery.addBindValue(Toolbox::itos(SystemLogger::instance()->errorsNum()));

    if(!DatabaseManager::instance()->exec(&statusQuery))
        LOG_ERROR("FAILURE\n\n");
}

QString DBStatusExportStrategy::description()
{
    return "database";
}
