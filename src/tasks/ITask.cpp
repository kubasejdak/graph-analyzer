/*
 *  Filename	: ITask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 05-04-2013
 */

#include "ITask.h"

ITask::ITask(int id)
{
    m_xmlId = id;
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

int ITask::id()
{
    return m_id;
}

QString ITask::name()
{
    return m_name;
}

QString ITask::type()
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

QString ITask::traitName()
{
	return m_traitName;
}

QString ITask::traitValue()
{
	return m_traitValue;
}

void ITask::setId(int id)
{
    m_id = id;
}

bool ITask::perform()
{
    /* update start time */
    m_startTime = m_timer.currentTime();
    m_timer.start();

    /* perform task */
    bool ret = performTask();

    /* update end time */
    m_endTime = m_timer.currentTime();

    /* update working time */
	m_workTime = QTime(0, 0).addMSecs(m_timer.elapsed());

    return ret;
}
