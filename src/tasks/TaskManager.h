#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <tasks/TaskQueue.h>
#include <tasks/TaskResolver.h>
#include <utils/XMLParser.h>

class TaskManager {
public:
	TaskManager() {}
	virtual ~TaskManager() {}

	bool collectTasks();
	bool executeTasks();

private:
	TaskQueue m_queue;
	TaskResolver m_resolver;
	XMLParser m_xmlParser;
};

#endif /* TASKMANAGER_H */
