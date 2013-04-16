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
    /* get next task_id number */
    if(task->id() == -1) {
        int taskId = DatabaseManager::instance()->sequenceValue("analyze_sample_id_seq");
        task->setId(taskId);
    }
    else {
        QSqlQuery deleteQuery(DatabaseManager::instance()->database());
        deleteQuery.prepare("DELETE FROM tasks_task WHERE name = ?");
        deleteQuery.addBindValue(task->name());

        if(!DatabaseManager::instance()->exec(&deleteQuery))
            LOG_ERROR("FAILURE\n\n");
    }

    QSqlQuery statusQuery(DatabaseManager::instance()->database());
    statusQuery.prepare("INSERT INTO tasks_task VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    statusQuery.addBindValue(task->id());
    statusQuery.addBindValue(task->name());
    statusQuery.addBindValue(task->type());
    statusQuery.addBindValue(task->startTime().toString("HH:mm"));
    statusQuery.addBindValue(task->endTime().toString("HH:mm"));
    statusQuery.addBindValue(task->workTime().toString("HH:mm"));
    statusQuery.addBindValue(task->errors());
    statusQuery.addBindValue(task->progress());

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
