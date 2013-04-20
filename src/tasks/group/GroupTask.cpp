/*
 *  Filename	: GroupTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "GroupTask.h"

GroupTask::GroupTask()
{
    m_type = "group";
}

bool GroupTask::performTask()
{
	return true;
}

void GroupTask::updateStatus()
{
}

bool GroupTask::readConfigXML(QDomElement taskNode)
{
	return false;
}
