/*
 *  Filename	: TaskManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "TaskManager.h"
#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>
#include <tasks/ITask.h>

#include <QFile>

bool TaskManager::collectTasks()
{
	if(!m_xmlParser.open(TASKS_FILE)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	QDomElement taskElement = m_xmlParser.root(ROOT_NODE);
	int taskId = 0;
	while(!taskElement.isNull()) {
		QString taskType = taskElement.attribute("type");

		ITask *task = m_resolver.createTask(taskType, taskId);
		m_queue.insert(task);
        SystemLogger::instance()->exportStatus(task);

		taskElement = taskElement.nextSiblingElement(ROOT_NODE);
		++taskId;
	}

	m_xmlParser.close();
	LOG("SUCCESS\n\n");
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

    removeTasks();
	LOG("SUCCESS\n\n");
	return true;
}

void TaskManager::removeTasks()
{
    XMLParser xmlParser;

    if(!xmlParser.open(TASKS_FILE)) {
        LOG_ERROR("FAILURE\n\n");
        return;
    }

    if(!xmlParser.hasRoot(ROOT_NODE)) {
        xmlParser.close();
        LOG_ERROR("FAILURE\n\n");
        return;
    }

    QDomElement taskElement = m_xmlParser.root(ROOT_NODE);
    while(taskElement.isNull() == false) {
        QDomElement tmpElement = taskElement;
        taskElement = taskElement.nextSiblingElement(ROOT_NODE);
        xmlParser.removeRoot(tmpElement);
    }

    xmlParser.close();
}
