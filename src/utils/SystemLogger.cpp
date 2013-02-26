/*
 * Filename		: SystemLogger.cpp
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#include "SystemLogger.h"

SystemLogger::SystemLogger()
{
    m_status = "idle";
    m_error = "no error";
    m_logLevel = 0;
    m_logFile = "";

}

void SystemLogger::setStatus(QString status)
{
    m_status = status;
}

void SystemLogger::setError(QString error)
{
    m_error = error;
}

QString SystemLogger::status()
{
    return m_status;
}

QString SystemLogger::error()
{
    return m_error;
}

void SystemLogger::setLogLevel(int level)
{
    m_logLevel = level;
}

void SystemLogger::setLogFile(QString filename)
{
    m_logFile = filename;
}

void SystemLogger::log(QString file, QString func, int line, QString msg)
{
    if(m_logLevel == 0)
        return;

    /* prepare string */
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString m = QString("[%1 %2] ").arg(date).arg(time);

    switch(m_logLevel) {
    case 1:
        m += msg;
        break;
    case 2:
        m += QString("FUNC: %1, LINE: %2 %3").arg(func).arg(line).arg(msg);
        break;
    case 3:
        m += QString("FILE: %1, FUNC: %2, LINE: %3 %4").arg(file).arg(func).arg(line).arg(msg);
        break;
    default:
        break;
    }

    /* print to console */
    clog << m.toStdString();

    /* print to file */
    if(m_logFile != "") {
        QFile f(m_logFile);
        f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
        if(!f.isOpen())
            return;

        f.write(m.toUtf8());
        f.close();
    }
}

void SystemLogger::logError(QString file, QString func, int line, QString msg)
{
    if(m_logLevel == 0)
        return;

    /* prepare string */
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString m = QString("[%1 %2] ").arg(date).arg(time);

    switch(m_logLevel) {
    case 1:
        m += QString("ERROR: %1").arg(msg);
        break;
    case 2:
        m += QString("FUNC: %1, LINE: %2 ERROR: %3").arg(func).arg(line).arg(msg);
        break;
    case 3:
        m += QString("FILE: %1, FUNC: %2, LINE: %3 ERROR: %4").arg(file).arg(func).arg(line).arg(msg);
        break;
    default:
        break;
    }

    /* print to console */
    clog << m.toStdString();

    /* print to file */
    if(m_logFile != "") {
        QFile f(m_logFile);
        f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
        if(!f.isOpen())
            return;

        f.write(m.toUtf8());
        f.close();
    }
}

void SystemLogger::clearError()
{
    m_error = "no error";
}

void SystemLogger::checkFileSize()
{
	QFile f(m_logFile);

	/* if size is greater than 1GB */
	if(f.size() > 1024*1024*1024) {
		f.resize(0);
		LOG("log file is too big: [%lld], truncating to 0\n", f.size());
	}
}
