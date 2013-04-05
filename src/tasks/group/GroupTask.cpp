/*
 *  Filename	: GroupTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "GroupTask.h"

GroupTask::GroupTask(int id) : ITask(id)
{
}

bool GroupTask::performTask()
{
	return true;
}
