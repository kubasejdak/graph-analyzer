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

	QDate from();
	QDate until();
	std::list<std::string> taskFiles();

	void incrementGroupedsamples();
	void incrementFoundGroups();

	virtual void updateStatus();

private:
	virtual bool performTask();
	virtual bool readConfigXML(QDomElement taskNode);

	bool load();
	bool exportResults(ExploitGroupHandle g);

	GroupManager m_groupManager;
    Group::AlgorithmContext m_context;

	// task specific info
	bool m_duplicate;
	std::string m_inputStrategy;
	std::string m_algorithm;
	int m_foundGroups;
	int m_groupedSamples;

};

#endif /* GROUPTASK_H */
