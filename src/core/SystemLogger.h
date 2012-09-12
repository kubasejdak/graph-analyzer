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
#include <boost/current_function.hpp>
#include <cstdarg>
#include <cstdio>
#include <toolbox.h>
using namespace std;

#define	LOG(fmt, ...) 		SystemLogger::getInstance()->log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, fmt, __VA_ARGS__)
#define	LOG_ERROR(fmt, ...)	SystemLogger::getInstance()->logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, fmt, __VA_ARGS__)

class SystemLogger {
public:
	virtual ~SystemLogger();
	static SystemLogger *getInstance()
	{
		static SystemLogger instance;
		return &instance;
	}

	/* error and status */
	void setStatus(SystemStatus status);
	void setError(SystemError error);

	SystemStatus getStatus();
	SystemError getError();

	string mapStatus(SystemStatus status);
	string mapError(SystemError error);

	/* logging */
	void setLogLevel(int level);
	void setLogFile(string filename);

	void log(string file, string func, int line, string fmt, ...);
	void logError(string file, string func, int line, string fmt, ...);

private:
	SystemLogger();

	SystemStatus status;
	SystemError error;
	map<SystemStatus, string> status_map;
	map<SystemError, string> error_map;

	int logging_level;
	string file;
};

#endif /* SYSTEMLOGGER_H_ */
