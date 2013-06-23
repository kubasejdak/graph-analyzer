/*
 *  Filename	: TaskQueue.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "TaskQueue.h"

#include <vector>
#include <tasks/ITask.h>

using namespace std;

TaskQueue::~TaskQueue()
{
	while(m_tasks.empty() == false) {
		delete m_tasks.back();
		m_tasks.pop_back();
	}
}

void TaskQueue::insert(ITask *task)
{
	m_tasks.push_back(task);
}

ITask *TaskQueue::get()
{
	if(m_tasks.size() == 0)
		return NULL;

	ITask *ret = m_tasks.front();
	m_tasks.erase(m_tasks.begin());

	return ret;
}

int TaskQueue::size()
{
	return m_tasks.size();
}
