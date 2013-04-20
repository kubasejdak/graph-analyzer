/*
 *  Filename	: TaskResolver.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef TASKRESOLVER_H
#define TASKRESOLVER_H

#include <QString>

#include <tasks/ITask.h>

class TaskResolver {
public:
	TaskResolver() {}
	virtual ~TaskResolver() {}

	ITask *createTask(QString taskName);
};

#endif /* TASKRESOLVER_H */
