/*
 *  Filename	: TaskResolver.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "TaskResolver.h"

#include <string>

#include <tasks/ITask.h>
#include <tasks/analyze/AnalyzeTask.h>
#include <tasks/group/GroupTask.h>

using namespace std;

ITask *TaskResolver::createTask(string taskName)
{
	if(taskName == "analyze")
		return new AnalyzeTask();

	if(taskName == "group")
		return new GroupTask();

	return NULL;
}
