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
	QSqlQuery statusQuery(DatabaseManager::instance()->database());

    /* get next task_id number */
    if(task->id() == -1) {
        int taskId = DatabaseManager::instance()->sequenceValue("tasks_task_id_seq");
        task->setId(taskId);

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
    }
    else {
		statusQuery.prepare("UPDATE tasks_task SET name = ?, type = ?, start_time = ?, end_time = ?, work_time = ?, errors = ?, finished = ?, progress = ? WHERE id = ?");
		statusQuery.addBindValue(task->name());
		statusQuery.addBindValue(task->type());
		statusQuery.addBindValue(task->startTime().toString("HH:mm"));
		statusQuery.addBindValue(task->endTime().toString("HH:mm"));
		statusQuery.addBindValue(task->workTime().toString("HH:mm"));
		statusQuery.addBindValue(task->errors());
		statusQuery.addBindValue(task->isFinished());
		statusQuery.addBindValue(task->progress());
		statusQuery.addBindValue(task->id());
    }

    if(!DatabaseManager::instance()->exec(&statusQuery))
        LOG_ERROR("FAILURE\n\n");
}

QString DBStatusExportStrategy::description()
{
    return "database";
}
