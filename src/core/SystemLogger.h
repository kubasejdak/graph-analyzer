/*
 * Filename		: SystemLogger.h
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#ifndef SYSTEMLOGGER_H_
#define SYSTEMLOGGER_H_

enum SystemStatus {
	IDLE,
	LOADING,
	EMULATING,
	ANALYZING,
	GENERATING_OUTPUT
};

enum SystemError {
	NO_ERROR,
	CANNOT_HANDLE_FILE,
	EMULATION_FAILED,
	ANALYZING_FAILED,
	CHANGE_DIR_FAILED,
	TCPFLOW_FAILED,
	OPEN_DIR_FAILED,
	UNLINK_FAILED,
	GRAPH_DRAW_FAILED,
	OUTPUT_FAILED
};

/* standard headers */
#include <string>
#include <map>
using namespace std;

class SystemLogger {
public:
	virtual ~SystemLogger();
	static SystemLogger *getInstance()
	{
		static SystemLogger instance;
		return &instance;
	}

	void setStatus(SystemStatus status);
	void setError(SystemError error);

	SystemStatus getStatus();
	SystemError getError();
	string mapStatus(SystemStatus status);
	string mapError(SystemError error);

private:
	SystemLogger();

	SystemStatus status;
	SystemError error;
	map<SystemStatus, string> status_map;
	map<SystemError, string> error_map;
};

#endif /* SYSTEMLOGGER_H_ */
