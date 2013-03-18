#include "GroupTask.h"

GroupTask::GroupTask(int id) : ITask(id)
{
}

bool GroupTask::perform()
{
	return true;
}
