#include "TaskManager.h"
#include <utils/SystemLogger.h>

#include <QFile>

bool TaskManager::collectTasks()
{
	if(!m_xmlParser.open(TASKS_FILE))
		return false;

	QDomElement taskElement = m_xmlParser.root(ROOT_NODE);
	int taskId = 0;
	while(!taskElement.isNull()) {
		QString taskType = taskElement.attribute("type");

		ITask *task = m_resolver.createTask(taskType, taskId);
		m_queue.insert(task);

		taskElement = taskElement.nextSiblingElement(ROOT_NODE);
		++taskId;
	}

	m_xmlParser.close();
	return true;
}

bool TaskManager::executeTasks()
{
	while(m_queue.size() != 0) {
		ITask *task = m_queue.get();

		if(!task->perform())
			LOG_ERROR("executing task failed\n");

		delete task;
	}

	QFile(TASKS_FILE).remove();
	return true;
}
