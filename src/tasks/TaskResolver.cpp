#include "TaskResolver.h"
#include <tasks/analyze/AnalyzeTask.h>
#include <tasks/group/GroupTask.h>

ITask *TaskResolver::createTask(QString taskName, int id)
{
	if(taskName == "analyze")
		return new AnalyzeTask(id);

	if(taskName == "group")
		return new GroupTask(id);

	return NULL;
}
