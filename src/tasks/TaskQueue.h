#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <QVector>
#include <tasks/ITask.h>

class TaskQueue {
public:
	TaskQueue() {}
	virtual ~TaskQueue();

	/* access */
	void insert(ITask *task);
	ITask *get();

	/* utils */
	int size();

private:
	QVector<ITask *> m_tasks;
};

#endif /* TASKQUEUE_H */
