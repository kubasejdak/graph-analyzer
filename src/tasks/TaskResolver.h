/*
 *  Filename	: TaskResolver.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef TASKRESOLVER_H
#define TASKRESOLVER_H

#include <string>

class ITask;

class TaskResolver {
public:
	TaskResolver() {}
	virtual ~TaskResolver() {}

	ITask *createTask(std::string taskName);
};

#endif /* TASKRESOLVER_H */
