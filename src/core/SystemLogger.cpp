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
    return m_error;
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

void SystemLogger::log(QString msg_file, QString func, int line, QString fmt, ...)
{
    if(m_logLevel == 0)
		return;

    /* prepare string */
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString m = QString("[%1 %2] ").arg(date).arg(time);
	va_list arg_ptr;
	va_start(arg_ptr, fmt);

    switch(m_logLevel) {
    case 1:
        m += fmt;
		break;
	case 2:
        m += QString("FUNC: %1, LINE: %2 %3").arg(func).arg(line).arg(fmt);
		break;
	case 3:
        m += QString("FILE: %1, FUNC: %2, LINE: %3 %4").arg(msg_file).arg(func).arg(QString().setNum(line)).arg(fmt);
		break;
	default:
		break;
	}

    /* empty line feature */
    if(fmt == "\n")
        m = "\n\n";

    /* print to console */
    vfprintf(stderr, m.toStdString().c_str(), arg_ptr);

    /* print to file */
    if(m_logFile != "") {

        FILE *f = fopen(m_logFile.toStdString().c_str(), "a");
        if(f == NULL) {
            va_end(arg_ptr);
            return;
        }

        vfprintf(f, m.toStdString().c_str(), arg_ptr);
        fclose(f);
    }
	va_end(arg_ptr);
}

void SystemLogger::logError(QString msg_file, QString func, int line, QString fmt, ...)
{
    if(m_logLevel == 0)
        return;

    /* prepare string */
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString m = QString("[%1 %2] ").arg(date).arg(time);
    va_list arg_ptr;
    va_start(arg_ptr, fmt);

    switch(m_logLevel) {
    case 1:
        m += QString("ERROR: %1").arg(fmt);
        break;
    case 2:
        m += QString("FUNC: %1, LINE: %2, ERROR: %3").arg(func).arg(line).arg(fmt);
        break;
    case 3:
        m += QString("FILE: %1, FUNC: %2, LINE: %3, ERROR: %4").arg(msg_file).arg(func).arg(QString().setNum(line)).arg(fmt);
        break;
    default:
        break;
    }

    /* empty line feature */
    if(fmt == "\n")
        m = "\n\n";

    /* print to console */
    vfprintf(stderr, m.toStdString().c_str(), arg_ptr);

    /* print to file */
    if(m_logFile != "") {

        FILE *f = fopen(m_logFile.toStdString().c_str(), "a");
        if(f == NULL) {
            va_end(arg_ptr);
            return;
        }

        vfprintf(f, m.toStdString().c_str(), arg_ptr);
        fclose(f);
    }
    va_end(arg_ptr);
}

void SystemLogger::clearError()
{
    m_error = "no error";
}
