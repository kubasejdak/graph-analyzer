/*
 * Filename	: SystemLogger.cpp
 * Author	: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#include "SystemLogger.h"

SystemLogger::SystemLogger() {
	status = IDLE;
	error = NO_ERROR;
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
