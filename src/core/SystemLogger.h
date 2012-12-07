/*
 * Filename		: SystemLogger.h
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#ifndef SYSTEMLOGGER_H_
#define SYSTEMLOGGER_H_

#include <QString>
#include <QDate>
#include <QTime>
#include <QFile>
#include <boost/current_function.hpp>
#include <iostream>
using namespace std;

#define	LOG(fmt, args...)           SystemLogger::instance()->log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, QString().sprintf(fmt, ##args))
#define	LOG_ERROR(fmt, args...)     SystemLogger::instance()->logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, QString().sprintf(fmt, ##args))

class SystemLogger {
public:
    static SystemLogger *instance()
	{
        static SystemLogger obj;
        return &obj;
	}

	/* error and status */
    void setStatus(QString status);
    void setError(QString error);
    void clearError();

    QString status();
    QString error();

	/* logging */
	void setLogLevel(int level);
    void setLogFile(QString filename);

    void log(QString file, QString func, int line, QString msg);
    void logError(QString file, QString func, int line, QString msg);

	/* utility */
	void checkFileSize();

private:
	SystemLogger();

    QString m_status;
    QString m_error;

    int m_logLevel;
    QString m_logFile;
};

#endif /* SYSTEMLOGGER_H_ */
