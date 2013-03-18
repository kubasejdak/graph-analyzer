#include "TaskManager.h"
#include <utils/XMLParser.h>
#include <utils/SystemLogger.h>

bool TaskManager::collectTasks()
{
	return true;
}

bool TaskManager::executeTasks()
{
	while(m_queue.size() != 0) {
		ITask *task = m_queue.get();

		if(!task->perform())
			LOG_ERROR("executing task failed\n");
	}

	return true;
}
