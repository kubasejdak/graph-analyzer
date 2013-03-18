#ifndef GROUPTASK_H
#define GROUPTASK_H

#include <tasks/ITask.h>

class GroupTask : public ITask {
public:
	GroupTask(int id);
	virtual ~GroupTask() {}

private:
	virtual bool perform();
};

#endif /* GROUPTASK_H */
