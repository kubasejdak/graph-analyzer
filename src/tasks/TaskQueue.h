/*
 *  Filename	: TaskQueue.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <vector>

class ITask;

class TaskQueue {
public:
	TaskQueue() {}
	virtual ~TaskQueue();

	// access
	void insert(ITask *task);
	ITask *get();

	// utils
	int size();

private:
	std::vector<ITask *> m_tasks;
};

#endif /* TASKQUEUE_H */
