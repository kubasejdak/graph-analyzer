/*
 *  Filename	: StatusExportStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 02-04-2013
 */

#include "StatusExportStrategy.h"
#include <utils/DatabaseManager.h>
#include <utils/SystemLogger.h>
#include <core/version.h>

void DBStatusExportStrategy::exportStatus(ITask *task)
{
    DatabaseManager::instance()->clearTable("options_systeminfo");
    int statusId = DatabaseManager::instance()->sequenceValue("options_systeminfo_id_seq");
    int progress = (task != NULL) ? task->progress() : 0;

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

void XMLStatusExportStrategy::exportStatus(ITask *task)
{
    if(!m_xmlParser.open(STATUS_FILE)) {
        LOG_ERROR("FAILURE\n\n");
        return;
    }

    if(m_xmlParser.hasRoot("SystemStatus")) {
        QDomElement s = m_xmlParser.root("SystemStatus");
        m_xmlParser.removeRoot(s);
    }

    QDomElement statusNode = m_xmlParser.createRoot("SystemStatus");
    int progress = (task != NULL) ? task->progress() : 0;

    m_xmlParser.createChild(statusNode, "Version").setAttribute("val", VERSION);
    m_xmlParser.createChild(statusNode, "Progress").setAttribute("val", progress);
    m_xmlParser.createChild(statusNode, "CurrentTask").setAttribute("name", SystemLogger::instance()->status());
    m_xmlParser.createChild(statusNode, "LastError").setAttribute("desc", SystemLogger::instance()->error());
    m_xmlParser.createChild(statusNode, "ErrorsNum").setAttribute("val", SystemLogger::instance()->errorsNum());;

    m_xmlParser.close();
}

QString XMLStatusExportStrategy::description()
{
    return "xml";
}
