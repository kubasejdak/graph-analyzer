/*
 * Filename	: SystemLogger.cpp
 * Author	: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#include "SystemLogger.h"

SystemLogger::SystemLogger() {
	status = IDLE;
	error = NO_ERROR;
	errorMap.clear();

	/* error strings mapping */
	errorMap[NO_ERROR] = "";
	errorMap[CANNOT_HANDLE_FILE] = "no appropriate input module";
	errorMap[EMULATION_FAILED] = "general emulation error";
	errorMap[ANALYZING_FAILED] = "general analyzing error";
	errorMap[CHANGE_DIR_FAILED] = "chdir function failed";
	errorMap[TCPFLOW_FAILED] = "external program 'tcpflow' failed";
	errorMap[OPEN_DIR_FAILED] = "opendir function failed";
	errorMap[UNLINK_FAILED] = "unlunk function failed";
	errorMap[GRAPH_DRAW_FAILED] = "drawing graph failed";

}

SystemLogger::~SystemLogger() {
}

void SystemLogger::setStatus(SystemStatus status) {
	this->status = status;
}

void SystemLogger::setError(SystemError error) {
	this->error = error;
}

SystemStatus SystemLogger::getStatus() {
	return status;
}

SystemError SystemLogger::getError() {
	return error;
}

string SystemLogger::mapError(SystemError error) {
	return errorMap[error];
}
