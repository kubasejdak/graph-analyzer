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
	QSqlQuery taskQuery(DatabaseManager::instance()->database());
	stringstream ss, ss2;

	// get next task_id number
    if(task->id() == -1) {
        int taskId = DatabaseManager::instance()->sequenceValue("tasks_task_id_seq");
        task->setId(taskId);

		// export status
		ss << "INSERT INTO tasks_task VALUES (" << task->id() << ", "
												<< "'" << task->name() << "', "
												<< "'" << task->type() << "', "
												<< "'" << task->startTime().toString("HH:mm").toStdString() << "', "
												<< "'" << task->endTime().toString("HH:mm").toStdString() << "', "
												<< "'" << task->workTime().toString("HH:mm").toStdString() << "', "
												<< task->errors() << ", "
												<< "'" << task->isFinished() << "', "
												<< "'" << task->displayTraitName() << "', "
												<< "'" << task->displayTraitValue() << "', "
												<< task->progress() << ")";

		taskQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&taskQuery))
			LOG_ERROR("FAILURE\n\n");

		// export traits
		ITask::TaskTraits::iterator it;
		for(it = task->traits().begin(); it != task->traits().end(); ++it) {
			int taskTraitId = DatabaseManager::instance()->sequenceValue("tasks_tasktrait_id_seq");
			ss2.str("");
			ss2 << "INSERT INTO tasks_tasktrait VALUES (" << taskTraitId
														  << ", '" << it->first << "'"
														  << ", '" << it->second << "'"
														  << ", " << task->id() << ")";

			QSqlQuery taskTraitQuery(DatabaseManager::instance()->database());
			taskTraitQuery.prepare(ss2.str().c_str());
			if(!DatabaseManager::instance()->exec(&taskTraitQuery))
				LOG_ERROR("FAILURE\n\n");
		}
    }
    else {
		// export status
		ss << "UPDATE tasks_task SET" << " name = '" << task->name() << "'"
									  << ", type = '" << task->type() << "'"
									  << ", start_time = '" << task->startTime().toString("HH:mm").toStdString() << "'"
									  << ", end_time = '" << task->endTime().toString("HH:mm").toStdString() << "'"
									  << ", work_time = '" << task->workTime().toString("HH:mm").toStdString() << "'"
									  << ", errors = " << task->errors()
									  << ", finished = '" << task->isFinished() << "'"
									  << ", trait_name = '" << task->displayTraitName() << "'"
									  << ", trait_value = '" << task->displayTraitValue() << "'"
									  << ", progress = " << task->progress()
									  << " WHERE id = " << task->id();

		taskQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&taskQuery))
			LOG_ERROR("FAILURE\n\n");

		// export traits
		ITask::TaskTraits::iterator it;
		for(it = task->traits().begin(); it != task->traits().end(); ++it) {
			ss2.str("");
			ss2 << "UPDATE tasks_tasktrait SET" << " value = '" << it->second << "'"
												<< " WHERE name = '" << it->first << "'"
												<< " AND task_id = " << task->id();

			QSqlQuery taskTraitQuery(DatabaseManager::instance()->database());
			taskTraitQuery.prepare(ss2.str().c_str());
			if(!DatabaseManager::instance()->exec(&taskTraitQuery))
				LOG_ERROR("FAILURE\n\n");
		}
    }
}

string DBStatusExportStrategy::description()
{
    return "database";
}
