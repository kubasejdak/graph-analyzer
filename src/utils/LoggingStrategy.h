/*
 *  Filename	: LoggingStrategy.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef LOGGINGSTRATEGY_H
#define LOGGINGSTRATEGY_H

#include <string>

class ILoggingStrategy {
public:
	ILoggingStrategy() {}
	virtual ~ILoggingStrategy() {}

	virtual void log(std::string message) = 0;
	virtual std::string description() = 0;
};

class FileLoggingStrategy : public ILoggingStrategy {
public:
	FileLoggingStrategy(std::string filename);
	virtual ~FileLoggingStrategy() {}

	virtual void log(std::string message);
	virtual std::string description();

private:
	std::string m_logFile;
};

class ConsoleLoggingStrategy : public ILoggingStrategy {
public:
	ConsoleLoggingStrategy() {}
	virtual ~ConsoleLoggingStrategy() {}

	virtual void log(std::string message);
	virtual std::string description();
};

#endif // LOGGINGSTRATEGY_H
