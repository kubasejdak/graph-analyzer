/*
 *  Filename	: ITask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#ifndef ITASK_H
#define ITASK_H

#define TASKS_FILE		"/var/www/jsejdak/GraphAnalyzerTasks.xml"
#define ROOT_NODE		"Task"

#include <QList>
#include <QString>
#include <utils/XMLParser.h>
#include <utils/SystemLogger.h>
#include <core/ExploitSample.h>

class ITask {
public:
	ITask(int id) : m_id(id) { m_errors = 0; }
	virtual ~ITask() {}

	virtual bool perform() = 0;

protected:
	XMLParser m_xmlParser;
	int m_id;
	int m_errors;

	SampleContainer m_samples;
	QList<QString> m_outStrategies;
};

#endif /* ITASK_H */
