/*
 * Filename		: SystemLogger.cpp
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#include "SystemLogger.h"

SystemLogger::SystemLogger()
{
	status = IDLE;
	error = NO_ERROR;
	status_map.clear();
	error_map.clear();

	/* status strings mapping */
	status_map[IDLE] = "idle";
	status_map[LOADING] = "loading";
	status_map[EMULATING] = "emulating";
	status_map[ANALYZING] = "analyzing";
	status_map[GENERATING_OUTPUT] = "generating output";

	/* error strings mapping */
	error_map[NO_ERROR] = "";
	error_map[CANNOT_HANDLE_FILE] = "no appropriate input module";
	error_map[EMULATION_FAILED] = "general emulation error";
	error_map[ANALYZING_FAILED] = "general analyzing error";
	error_map[CHANGE_DIR_FAILED] = "chdir function failed";
	error_map[TCPFLOW_FAILED] = "external program 'tcpflow' failed";
	error_map[OPEN_DIR_FAILED] = "opendir function failed";
	error_map[UNLINK_FAILED] = "unlunk function failed";
	error_map[GRAPH_DRAW_FAILED] = "drawing graph failed";
	error_map[OUTPUT_FAILED] = "generating output failed";

}

SystemLogger::~SystemLogger()
{
}

void SystemLogger::setStatus(SystemStatus status)
{
	this->status = status;
}

void SystemLogger::setError(SystemError error)
{
	this->error = error;
}

SystemStatus SystemLogger::getStatus()
{
	return status;
}

SystemError SystemLogger::getError()
{
	return error;
}

string SystemLogger::mapStatus(SystemStatus status)
{
	return status_map[status];
}

string SystemLogger::mapError(SystemError error)
{
	return error_map[error];
}
