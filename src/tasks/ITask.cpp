/*
 *  Filename	: ITask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 05-04-2013
 */

#include "ITask.h"

ITask::ITask(int id)
{
    m_id = id;
    m_errors = 0;
    m_progress = 0;

    m_name = "<no_name>";
}

int ITask::errors()
{
    return m_errors;
}

int ITask::progress()
{
    return m_progress;
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
    m_workTime = m_startTime.addMSecs(m_timer.elapsed());

    return ret;
}
