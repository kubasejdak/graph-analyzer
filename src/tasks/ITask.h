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
#include <core/ExploitSample.h>

class ITask {
public:
    ITask(int id);
	virtual ~ITask() {}

    bool perform();

    /* for exporting status purpose */
    int id();
    QString name();
    QString type();
    QTime startTime();
    QTime endTime();
    QTime workTime();
    int errors();
    int progress();
    bool isFinished();
	QString traitName();
	QString traitValue();

    void setId(int id);

protected:
    virtual bool performTask() = 0;

    XMLParser m_xmlParser;              /* provides parser for XML configuration file */
    SampleContainer m_samples;          /* samples that are used in task */
    QList<QString> m_exportStrategies;  /* strategies for exporting task results */
    QTime m_timer;                      /* provides time functions */
    int m_xmlId;                        /* number in XML */

    /* task general info */
    int m_id;                           /* task id */
    QString m_name;                     /* task name defined by user */
    QString m_type;                     /* task type */
    QTime m_startTime;                  /* when task was started */
    QTime m_endTime;                    /* when task was ended */
    QTime m_workTime;                   /* how long did task work */
    int m_errors;                       /* number of errors that occured during task */
    int m_progress;                     /* current progress of task */
	QString m_traitName;				/* task specific trait name */
	QString m_traitValue;				/* task specific trait value */
    bool m_finished;                    /* is task finished */

};

#endif /* ITASK_H */
