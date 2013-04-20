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
#include <QDomElement>

bool TaskManager::collectTasks()
{
	if(!m_xmlParser.open(TASKS_FILE)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	QDomElement taskNode = m_xmlParser.root(ROOT_NODE);
	while(taskNode.isNull() == false) {
		QString taskType = taskNode.attribute("type");

		ITask *task = m_resolver.createTask(taskType);

		task->readConfigXML(taskNode);
		m_queue.insert(task);
        SystemLogger::instance()->exportStatus(task);

		taskNode = taskNode.nextSiblingElement(ROOT_NODE);
	}

	m_xmlParser.close();
	m_xmlParser.clear(TASKS_FILE);
	LOG("SUCCESS\n\n");
	return true;
}

bool TaskManager::executeTasks()
{
	while(m_queue.size() != 0) {
		ITask *task = m_queue.get();

        if(task->perform() == false)
			LOG_ERROR("executing task failed\n");

        SystemLogger::instance()->exportStatus(task);
		delete task;
	}

	LOG("SUCCESS\n\n");
	return true;
}
