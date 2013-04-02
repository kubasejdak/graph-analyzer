/*
 * Filename		: SystemLogger.h
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#ifndef SYSTEMLOGGER_H_
#define SYSTEMLOGGER_H_

#include <QString>
#include <QList>
#include <boost/current_function.hpp>

#include <utils/LoggingStrategy.h>
#include <utils/StatusExportStrategy.h>
#include <utils/XMLParser.h>

#define	LOG(fmt, args...)           SystemLogger::instance()->log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, QString().sprintf(fmt, ##args))
#define	LOG_ERROR(fmt, args...)     SystemLogger::instance()->logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, QString().sprintf(fmt, ##args))

class SystemLogger {
public:
	~SystemLogger();
    static SystemLogger *instance()
	{
		static SystemLogger m_instance;
		return &m_instance;
	}

	/* error and status */
    void setStatus(QString status);
    QString status();

    void setError(QString error);
    void clearError();
    QString error();
    int errorsNum();

    /* logging */
    void log(QString file, QString func, int line, QString msg);
    void logError(QString file, QString func, int line, QString msg);

    /* exporting status */
    void exportStatus(int progress = 0);

private:
	SystemLogger();
	bool readConfigXML();
	void listOptions();

    QString m_status;
    QString m_error;
    int m_allErrors;

    int m_logLevel;
    QList<ILoggingStrategy *> m_logStrategies;
    QList<IStatusExportStrategy *> m_statusStrategies;

	XMLParser m_xmlParser;
};

#endif /* SYSTEMLOGGER_H_ */
