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
	error_map[NO_ERROR] = "no error";
	error_map[CANNOT_HANDLE_FILE] = "no appropriate input module";
	error_map[EMULATION_FAILED] = "general emulation error";
	error_map[ANALYZING_FAILED] = "general analyzing error";
	error_map[CHANGE_DIR_FAILED] = "chdir function failed";
	error_map[TCPFLOW_FAILED] = "external program 'tcpflow' failed";
	error_map[OPEN_DIR_FAILED] = "opendir function failed";
	error_map[UNLINK_FAILED] = "unlunk function failed";
	error_map[GRAPH_DRAW_FAILED] = "drawing graph failed";
	error_map[OUTPUT_FAILED] = "generating output failed";

	logging_level = 0;
	log_file = "/home/kuba/analyzer_log";

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

void SystemLogger::setLogLevel(int level)
{
	logging_level = level;
}

void SystemLogger::setLogFile(string filename)
{
	log_file = filename;
}

void SystemLogger::log(string msg_file, string func, int line, string fmt, ...)
{
	if(logging_level == 0)
		return;

	FILE *f = fopen(log_file.c_str(), "a");
	if(f == NULL)
		return;

	string m = "";
	va_list arg_ptr;
	va_start(arg_ptr, fmt);

	switch(logging_level) {
	case 1:
		m.append(fmt);
		break;
	case 2:
		m.append("FUNCTION: ").append(func).append(", ").append(fmt);
		break;
	case 3:
		m.append("FILE: ").append(msg_file).append(", FUNCTION: ").append(func);
		m.append(", LINE: ").append(itos(line)).append(", ").append(fmt);
		break;
	default:
		break;
	}

	vfprintf(f, m.c_str(), arg_ptr);
	fclose(f);
	va_end(arg_ptr);
}

void SystemLogger::logError(string msg_file, string func, int line, string fmt, ...)
{
	if(logging_level == 0)
		return;

	FILE *f = fopen(log_file.c_str(), "a");
	if(f == NULL)
		return;

	string m = "ERROR: ";
	va_list arg_ptr;
	va_start(arg_ptr, fmt);

	switch(logging_level) {
	case 1:
		m.append(fmt);
		break;
	case 2:
		m.append("FUNCTION: ").append(func).append(", ").append(fmt);
		break;
	case 3:
		m.append("FILE: ").append(msg_file).append(", FUNCTION: ").append(func);
		m.append(", LINE: ").append(itos(line)).append(", ").append(fmt);
		break;
	default:
		break;
	}

	vfprintf(stdout, m.c_str(), arg_ptr);
	vfprintf(f, m.c_str(), arg_ptr);
	fclose(f);
	va_end(arg_ptr);
}
