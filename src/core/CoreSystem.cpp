/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

#include <string>

#include <core/version.h>
#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>
#include <tasks/TaskManager.h>

using namespace std;

CoreSystem::CoreSystem()
{
    SystemLogger::instance()->setStatus("idle");
    SystemLogger::instance()->clearError();

	LOG("==================================================================================================================\n");
	LOG("=                                              SYSTEM STARTED                                                     \n");
	LOG("=                                 created CoreSystem instance, version: [%s]                                      \n", VERSION);
	LOG("==================================================================================================================\n");
	LOG("SUCCESS\n\n");
}

bool CoreSystem::executeTasks()
{
	TaskManager taskManager;

	if(!taskManager.collectTasks()) {
		LOG_ERROR("collecting tasks failed\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	if(!taskManager.executeTasks()) {
		LOG_ERROR("executing tasks failed\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

string CoreSystem::status()
{
	return SystemLogger::instance()->status();
}

string CoreSystem::lastError()
{
	return SystemLogger::instance()->error();
}

string CoreSystem::version()
{
	return VERSION;
}
