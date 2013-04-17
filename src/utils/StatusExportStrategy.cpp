/*
 *  Filename	: StatusExportStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 02-04-2013
 */

#include "StatusExportStrategy.h"
#include <utils/DatabaseManager.h>
#include <utils/SystemLogger.h>

void DBStatusExportStrategy::exportStatus(ITask *task)
{
    /* get next task_id number */
    if(task->id() == -1) {
        int taskId = DatabaseManager::instance()->sequenceValue("tasks_task_id_seq");
        task->setId(taskId);
    }
    else {
        QSqlQuery deleteQuery(DatabaseManager::instance()->database());
        deleteQuery.prepare("DELETE FROM tasks_task WHERE id = ?");
        deleteQuery.addBindValue(task->id());

        if(!DatabaseManager::instance()->exec(&deleteQuery))
            LOG_ERROR("FAILURE\n\n");
    }

    QSqlQuery statusQuery(DatabaseManager::instance()->database());
    statusQuery.prepare("INSERT INTO tasks_task VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    statusQuery.addBindValue(task->id());
    statusQuery.addBindValue(task->name());
    statusQuery.addBindValue(task->type());
    statusQuery.addBindValue(task->startTime().toString("HH:mm"));
    statusQuery.addBindValue(task->endTime().toString("HH:mm"));
    statusQuery.addBindValue(task->workTime().toString("HH:mm"));
    statusQuery.addBindValue(task->errors());
    statusQuery.addBindValue(task->isFinished());
    statusQuery.addBindValue(task->progress());

    if(!DatabaseManager::instance()->exec(&statusQuery))
        LOG_ERROR("FAILURE\n\n");
}

QString DBStatusExportStrategy::description()
{
    return "database";
}
