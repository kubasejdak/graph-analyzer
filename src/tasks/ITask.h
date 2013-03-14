/*
 *  Filename	: ITask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#ifndef ITASK_H
#define ITASK_H

#define TASKS_FILE		"/var/www/jsejdak/GraphAnalyzerTasks.xml"

#include <utils/XMLParser.h>

class ITask {
public:
	ITask();
	virtual ~ITask();

	virtual bool perform() = 0;

protected:
	XMLParser m_xmlParser;
};

#endif /* ITASK_H */
