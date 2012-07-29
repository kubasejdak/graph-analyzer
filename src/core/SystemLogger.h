/*
 * Filename	: SystemLogger.h
 * Author	: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#ifndef SYSTEMLOGGER_H_
#define SYSTEMLOGGER_H_

enum SystemStatus {
	IDLE,
	ERROR,
	LOADING,
	EMULATING,
	ANALYZING
};

enum SystemError {
	NO_ERROR,
	CANNOT_HANDLE_FILE,
	EMULATION_FAILED,
	ANALYZING_FAILED
};

class SystemLogger {
public:
	virtual ~SystemLogger();
	static SystemLogger *getInstance() {
		static SystemLogger instance;
	    return &instance;
	}

	void setStatus(SystemStatus status);
	void setError(SystemError error);

	SystemStatus getStatus();
	SystemError getError();

private:
	SystemLogger();

	SystemStatus status;
	SystemError error;
};

#endif /* SYSTEMLOGGER_H_ */
