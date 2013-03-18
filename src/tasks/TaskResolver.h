#ifndef TASKRESOLVER_H
#define TASKRESOLVER_H

#include <QString>

#include <tasks/ITask.h>

class TaskResolver {
public:
	TaskResolver() {}
	virtual ~TaskResolver() {}

	ITask *createTask(QString taskName, int id);
};

#endif /* TASKRESOLVER_H */
