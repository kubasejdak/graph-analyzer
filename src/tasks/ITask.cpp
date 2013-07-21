/*
 *  Filename	: ITask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 05-04-2013
 */

#include "ITask.h"

#include <list>
#include <string>
#include <QTime> // TODO: change to some non-Qt
#include <QDate> // TODO: change to some non-Qt
#include <QDomElement>

#include <utils/XMLParser.h>
#include <core/ExploitSample.h>

using namespace std;

ITask::ITask()
{
    m_id = -1;
    m_errors = 0;
    m_progress = 0;
    m_finished = false;
    m_startTime = QTime(0, 0);
    m_endTime = QTime(0, 0);
    m_workTime = QTime(0, 0);

    m_name = "<no_name>";
    m_type = "<no_type>";
}

QDate ITask::from()
{
	return m_from;
}

QDate ITask::until()
{
	return m_until;
}

list<string> ITask::taskFiles()
{
	return m_taskFiles;
}

int ITask::id()
{
    return m_id;
}

string ITask::name()
{
    return m_name;
}

string ITask::type()
{
    return m_type;
}

QTime ITask::startTime()
{
    return m_startTime;
}

QTime ITask::endTime()
{
    return m_endTime;
}

QTime ITask::workTime()
{
    return m_workTime;
}

int ITask::errors()
{
    return m_errors;
}

int ITask::progress()
{
    return m_progress;
}

bool ITask::isFinished()
{
    return m_finished;
}

string ITask::displayTraitName()
{
	return m_displayTraitName;
}

string ITask::displayTraitValue()
{
	return m_displayTraitValue;
}

ITask::TaskTraits &ITask::traits()
{
	return m_traits;
}

void ITask::setId(int id)
{
    m_id = id;
}

bool ITask::perform()
{
	// update start time
    m_startTime = m_timer.currentTime();
    m_timer.start();

	// perform task
    bool ret = performTask();

	// update end time
    m_endTime = m_timer.currentTime();

	// update working time
	m_workTime = QTime(0, 0).addMSecs(m_timer.elapsed());

    return ret;
}
