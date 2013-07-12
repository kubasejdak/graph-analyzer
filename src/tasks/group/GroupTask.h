/*
 *  Filename	: GroupTask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef GROUPTASK_H
#define GROUPTASK_H

#include <list>
#include <string>
#include <QDate>
#include <QDomElement>

#include <core/ExploitGroup.h>
#include <tasks/ITask.h>
#include <tasks/group/GroupManager.h>
#include <tasks/group/modules/algorithms/AlgorithmContext.h>

class GroupTask : public ITask {
public:
	GroupTask();
	virtual ~GroupTask() {}

private:
    virtual bool performTask();
	bool exportResults(ExploitGroupHandle g);

	virtual void updateStatus();
	virtual bool readConfigXML(QDomElement taskNode);
    bool collectTaskSamples();

	GroupManager m_groupManager;
    Group::AlgorithmContext m_context;

	// task specific info
	std::list<std::string> m_taskFiles;
    bool m_override;
    QDate m_from;
    QDate m_until;
	std::string m_algorithm;
	int m_foundGroups;
	int m_groupedSamples;

};

#endif /* GROUPTASK_H */
