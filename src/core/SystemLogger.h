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
#include <boost/current_function.hpp>
#include <cstdarg>
#include <cstdio>

#include <core/Toolbox.h>

#define	LOG(fmt, args...) 		SystemLogger::instance()->log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, fmt, ##args)
#define	LOG_ERROR(fmt, args...)	SystemLogger::instance()->logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, fmt, ##args)

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

    void log(QString msg_file, QString func, int line, QString fmt, ...);
    void logError(QString msg_file, QString func, int line, QString fmt, ...);

private:
	SystemLogger();

    QString m_status;
    QString m_error;

    int m_logLevel;
    QString m_logFile;
};

#endif /* SYSTEMLOGGER_H_ */
