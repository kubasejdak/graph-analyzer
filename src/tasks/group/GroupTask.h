#ifndef GROUPTASK_H
#define GROUPTASK_H

#include <tasks/ITask.h>
#include <tasks/group/GroupManager.h>

class GroupTask : public ITask {
public:
	GroupTask(int id);
	virtual ~GroupTask() {}

private:
	virtual bool perform();

	GroupManager m_groupManager;
};

#endif /* GROUPTASK_H */
