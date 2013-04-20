/*
 *  Filename	: GroupTask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef GROUPTASK_H
#define GROUPTASK_H

#include <tasks/ITask.h>
#include <tasks/group/GroupManager.h>

class GroupTask : public ITask {
public:
	GroupTask();
	virtual ~GroupTask() {}

private:
    virtual bool performTask();
	virtual void updateStatus();
	virtual bool readConfigXML(QDomElement taskNode);

	GroupManager m_groupManager;
};

#endif /* GROUPTASK_H */
