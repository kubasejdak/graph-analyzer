#ifndef LOGGINGSTRATEGY_H
#define LOGGINGSTRATEGY_H

#include <QString>

class ILoggingStrategy {
public:
	ILoggingStrategy() {}
	virtual ~ILoggingStrategy() {}

	virtual void log(QString message) = 0;
	virtual QString description() = 0;
};

class FileLoggingStrategy : public ILoggingStrategy {
public:
	FileLoggingStrategy(QString filename);
	virtual ~FileLoggingStrategy() {}

	virtual void log(QString message);
	virtual QString description();

private:
	QString m_logFile;
};

class ConsoleLoggingStrategy : public ILoggingStrategy {
public:
	ConsoleLoggingStrategy() {}
	virtual ~ConsoleLoggingStrategy() {}

	virtual void log(QString message);
	virtual QString description();
};

#endif // LOGGINGSTRATEGY_H
