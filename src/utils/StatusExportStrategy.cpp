/*
 *  Filename	: StatusExportStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 02-04-2013
 */

#include "StatusExportStrategy.h"

#include <sstream>
#include <string>

#include <tasks/ITask.h>
#include <utils/DatabaseManager.h>
#include <utils/SystemLogger.h>

using namespace std;

void DBStatusExportStrategy::exportStatus(ITask *task)
{
	QSqlQuery statusQuery(DatabaseManager::instance()->database());
	stringstream ss;

	// get next task_id number
    if(task->id() == -1) {
        int taskId = DatabaseManager::instance()->sequenceValue("tasks_task_id_seq");
        task->setId(taskId);

		ss << "INSERT INTO tasks_task VALUES (" << task->id() << ", "
												<< "'" << task->name() << "', "
												<< "'" << task->type() << "', "
												<< "'" << task->startTime().toString("HH:mm").toStdString() << "', "
												<< "'" << task->endTime().toString("HH:mm").toStdString() << "', "
												<< "'" << task->workTime().toString("HH:mm").toStdString() << "', "
												<< task->errors() << ", "
												<< "'" << task->isFinished() << "', "
												<< "'" << task->traitName() << "', "
												<< "'" << task->traitValue() << "', "
												<< task->progress() << ")";
    }
    else {
		ss << "UPDATE tasks_task SET" << " name = '" << task->name() << "'"
									  << ", type = '" << task->type() << "'"
									  << ", start_time = '" << task->startTime().toString("HH:mm").toStdString() << "'"
									  << ", end_time = '" << task->endTime().toString("HH:mm").toStdString() << "'"
									  << ", work_time = '" << task->workTime().toString("HH:mm").toStdString() << "'"
									  << ", errors = " << task->errors()
									  << ", finished = '" << task->isFinished() << "'"
									  << ", trait_name = '" << task->traitName() << "'"
									  << ", trait_value = '" << task->traitValue() << "'"
									  << ", progress = " << task->progress()
									  << " WHERE id = " << task->id();
    }

	statusQuery.prepare(ss.str().c_str());
    if(!DatabaseManager::instance()->exec(&statusQuery))
        LOG_ERROR("FAILURE\n\n");
}

string DBStatusExportStrategy::description()
{
    return "database";
}
