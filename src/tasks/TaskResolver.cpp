/*
 *  Filename	: TaskResolver.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "TaskResolver.h"
#include <tasks/analyze/AnalyzeTask.h>
#include <tasks/group/GroupTask.h>

ITask *TaskResolver::createTask(QString taskName)
{
	if(taskName == "analyze")
		return new AnalyzeTask();

	if(taskName == "group")
		return new GroupTask();

	return NULL;
}
