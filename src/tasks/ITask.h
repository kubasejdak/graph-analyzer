/*
 *  Filename	: ITask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#ifndef ITASK_H
#define ITASK_H

#define TASKS_FILE		"/var/www/jsejdak/GraphAnalyzerTasks.xml"
#define ROOT_NODE		"Task"

#include <list>
#include <string>
#include <QTime> // TODO: change to some non-Qt

#include <core/ExploitSample.h>
#include <utils/XMLParser.h>

class QDomElement;

class ITask {
public:
	ITask();
	virtual ~ITask() {}

    bool perform();
	virtual bool readConfigXML(QDomElement taskNode) = 0;

    // for exporting status purpose
    int id();
	std::string name();
	std::string type();
    QTime startTime();
    QTime endTime();
    QTime workTime();
    int errors();
    int progress();
    bool isFinished();
	std::string traitName();
	std::string traitValue();

    void setId(int id);

protected:
	virtual bool performTask() = 0;				// perform specific task
	virtual void updateStatus() = 0;			// update task status

	XMLParser m_xmlParser;						// provides parser for XML configuration file
	SampleList m_samples;					// samples that are used in task
	std::list<std::string> m_exportStrategies;  // strategies for exporting task results
	QTime m_timer;								// provides time functions

    // task general info
	int m_id;									// task id
	std::string m_name;							// task name defined by user
	std::string m_type;							// task type
	QTime m_startTime;							// when task was started
	QTime m_endTime;							// when task was ended
	QTime m_workTime;							// how long did task work
	int m_errors;								// number of errors that occured during task
	int m_progress;								// current progress of task
	std::string m_traitName;					// task specific trait name
	std::string m_traitValue;					// task specific trait value
	bool m_finished;							// is task finished

};

#endif /* ITASK_H */
