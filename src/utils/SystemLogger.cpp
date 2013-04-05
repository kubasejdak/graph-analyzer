/*
 * Filename		: SystemLogger.cpp
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#include "SystemLogger.h"
#include <core/Options.h>
#include <tasks/ITask.h>
#include <utils/StatusExportStrategy.h>

#include <QDate>
#include <QTime>
#include <iostream>
using namespace std;

#define LOG_INTERNAL(fmt, args...)			log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, QString().sprintf(fmt, ##args))
#define LOG_INTERNAL_ERROR(fmt, args...)	logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, QString().sprintf(fmt, ##args))

SystemLogger::SystemLogger()
{
    m_status = "idle";
    m_error = "no error";
    m_allErrors = 0;
    m_logLevel = 0;

	if(!readConfigXML()) {
        m_logStrategies.push_back(new ConsoleLoggingStrategy());
        m_statusStrategies.push_back(new DBStatusExportStrategy());
		LOG_INTERNAL_ERROR("failed to read XML configuration, using default options\n");
	}

	listOptions();
}

SystemLogger::~SystemLogger()
{
	QList<ILoggingStrategy *>::iterator it;
    for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
		delete (*it);

    QList<IStatusExportStrategy *>::iterator it2;
    for(it2 = m_statusStrategies.begin(); it2 != m_statusStrategies.end(); ++it2)
        delete (*it2);
}

bool SystemLogger::readConfigXML()
{
	if(!m_xmlParser.open(CONFIG_FILE))
		return false;

	if(!m_xmlParser.hasRoot("Logging")) {
		m_xmlParser.close();
		return false;
	}

	QDomElement options = m_xmlParser.root("Logging");

	/* level */
	m_logLevel = m_xmlParser.child(options, "Level").text().toInt();

	/* logging strategy */
    if(m_xmlParser.hasChild(options, "LogStrategy")) {
        QDomElement f = m_xmlParser.child(options, "LogStrategy");
		while(!f.isNull()) {

			if(f.attribute("type") == "file") {
				QString filename = f.text();
                m_logStrategies.push_back(new FileLoggingStrategy(filename));
			}
			else if(f.attribute("type") == "console")
                m_logStrategies.push_back(new ConsoleLoggingStrategy());

            f = f.nextSiblingElement("LogStrategy");
		}
	}

    /* exporting status */
    if(m_xmlParser.hasChild(options, "StatusStrategy")) {
        QDomElement f = m_xmlParser.child(options, "StatusStrategy");
        while(!f.isNull()) {

            if(f.attribute("type") == "database")
                m_statusStrategies.push_back(new DBStatusExportStrategy());

            f = f.nextSiblingElement("StatusStrategy");
        }
    }

	m_xmlParser.close();
	return true;
}

void SystemLogger::listOptions()
{
	LOG_INTERNAL("logging settings:\n");
	LOG_INTERNAL("m_logLevel: [%d]\n", m_logLevel);
    QList<ILoggingStrategy *>::iterator it;
    for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
        LOG_INTERNAL("m_logStrategy: [%s]\n", (*it)->description().toStdString().c_str());
    QList<IStatusExportStrategy *>::iterator it2;
    for(it2 = m_statusStrategies.begin(); it2 != m_statusStrategies.end(); ++it2)
        LOG_INTERNAL("m_statusStrategy: [%s]\n", (*it2)->description().toStdString().c_str());
}

void SystemLogger::setStatus(QString status)
{
    m_status = status;
}

void SystemLogger::setError(QString error)
{
    m_error = error;
    ++m_allErrors;
}

QString SystemLogger::status()
{
    return m_status;
}

QString SystemLogger::error()
{
    return m_error;
}

int SystemLogger::errorsNum()
{
    return m_allErrors;
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

	QList<ILoggingStrategy *>::iterator it;
    for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
		(*it)->log(m);
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
}

void SystemLogger::exportStatus(ITask *currTask)
{
    QList<IStatusExportStrategy *>::iterator it;
    for(it = m_statusStrategies.begin(); it != m_statusStrategies.end(); ++it)
        (*it)->exportStatus(currTask);
}

void SystemLogger::clearError()
{
    m_error = "no error";
}
