/*
 *  Filename	: TaskQueue.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "TaskQueue.h"

TaskQueue::~TaskQueue()
{
	while(!m_tasks.empty()) {
		delete m_tasks.first();
		m_tasks.pop_front();
	}
}

void TaskQueue::insert(ITask *task)
{
	m_tasks.append(task);
}

ITask *TaskQueue::get()
{
	if(m_tasks.size() == 0)
		return NULL;

	ITask *ret = m_tasks.first();
	m_tasks.pop_front();

	return ret;
}

int TaskQueue::size()
{
	return m_tasks.size();
}
