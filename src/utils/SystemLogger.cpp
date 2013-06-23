/*
 * Filename		: SystemLogger.cpp
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#include "SystemLogger.h"

#include <iostream>
#include <cstdarg>
#include <sstream>
#include <string>
#include <list>
#include <boost/current_function.hpp>
#include <QDate>
#include <QTime>

#include <core/Options.h>
#include <tasks/ITask.h>
#include <utils/StatusExportStrategy.h>
#include <utils/DescriptionExportStrategy.h>
#include <utils/LoggingStrategy.h>
#include <utils/XMLParser.h>
#include <utils/Toolbox.h>

using namespace std;

#define LOG_INTERNAL(fmt, args...)			log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, Toolbox::formatToString(fmt, ##args))
#define LOG_INTERNAL_ERROR(fmt, args...)	logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, Toolbox::formatToString(fmt, ##args))

SystemLogger::SystemLogger()
{
    m_status = "idle";
    m_error = "no error";
    m_allErrors = 0;
    m_logLevel = 0;

	if(!readConfigXML()) {
        m_logStrategies.push_back(new ConsoleLoggingStrategy());
        m_statusStrategies.push_back(new DBStatusExportStrategy());
        m_descriptionStrategies.push_back(new XMLDescriptionExportStrategy());
		LOG_INTERNAL_ERROR("failed to read XML configuration, using default options\n");
	}

	listOptions();
}

SystemLogger::~SystemLogger()
{
	list<ILoggingStrategy *>::iterator it;
    for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
		delete (*it);

	list<IStatusExportStrategy *>::iterator it2;
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

	// level
    m_logLevel = m_xmlParser.child(options, "Level").attribute("val").toInt();

	// logging strategy
    if(m_xmlParser.hasChild(options, "LogStrategy")) {
        QDomElement f = m_xmlParser.child(options, "LogStrategy");
		while(!f.isNull()) {

			if(f.attribute("type") == "file") {
				string filename = f.attribute("path").toStdString();
                m_logStrategies.push_back(new FileLoggingStrategy(filename));
			}
			else if(f.attribute("type") == "console")
                m_logStrategies.push_back(new ConsoleLoggingStrategy());

            f = f.nextSiblingElement("LogStrategy");
		}
	}

	// exporting status
    if(m_xmlParser.hasChild(options, "StatusStrategy")) {
        QDomElement f = m_xmlParser.child(options, "StatusStrategy");
        while(!f.isNull()) {

            if(f.attribute("type") == "database")
                m_statusStrategies.push_back(new DBStatusExportStrategy());

            f = f.nextSiblingElement("StatusStrategy");
        }
    }

	// exporting description
    if(m_xmlParser.hasChild(options, "DescriptionStrategy")) {
        QDomElement f = m_xmlParser.child(options, "DescriptionStrategy");
        while(!f.isNull()) {

            if(f.attribute("type") == "xml")
                m_descriptionStrategies.push_back(new XMLDescriptionExportStrategy());

            f = f.nextSiblingElement("DescriptionStrategy");
        }
    }

	m_xmlParser.close();
	return true;
}

void SystemLogger::listOptions()
{
	LOG_INTERNAL("logging settings:\n");
	LOG_INTERNAL("m_logLevel: [%d]\n", m_logLevel);
	list<ILoggingStrategy *>::iterator it;
    for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
		LOG_INTERNAL("m_logStrategy: [%s]\n", (*it)->description().c_str());
	list<IStatusExportStrategy *>::iterator it2;
    for(it2 = m_statusStrategies.begin(); it2 != m_statusStrategies.end(); ++it2)
		LOG_INTERNAL("m_statusStrategy: [%s]\n", (*it2)->description().c_str());
	list<IDescriptionExportStrategy *>::iterator it3;
    for(it3 = m_descriptionStrategies.begin(); it3 != m_descriptionStrategies.end(); ++it3)
		LOG_INTERNAL("m_descriptionStrategy: [%s]\n", (*it3)->description().c_str());
}

void SystemLogger::setStatus(string status)
{
    m_status = status;
}

void SystemLogger::setError(string error)
{
    m_error = error;
    ++m_allErrors;
}

string SystemLogger::status()
{
    return m_status;
}

string SystemLogger::error()
{
    return m_error;
}

int SystemLogger::errorsNum()
{
    return m_allErrors;
}

void SystemLogger::log(string file, string func, int line, string msg)
{
    if(m_logLevel == 0)
        return;

	// prepare string
	string date = QDate::currentDate().toString("dd.MM.yyyy").toStdString();
	string time = QTime::currentTime().toString("hh:mm:ss").toStdString();
	stringstream ss;
	ss << "[" << date << " " << time << "] ";
	string m;

    switch(m_logLevel) {
    case 1:
		ss << msg;
        break;
    case 2:
		ss << "FUNC: " << func << ", LINE: " << line << " " << msg;
        break;
    case 3:
		ss << "FILE: " << file << "FUNC: " << func << ", LINE: " << line << " " << msg;
        break;
    default:
        break;
    }

	m = ss.str();

	list<ILoggingStrategy *>::iterator it;
    for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
		(*it)->log(m);
}

void SystemLogger::logError(string file, string func, int line, string msg)
{
    if(m_logLevel == 0)
        return;

	// prepare string
	string date = QDate::currentDate().toString("dd.MM.yyyy").toStdString();
	string time = QTime::currentTime().toString("hh:mm:ss").toStdString();
	stringstream ss;
	ss << "[" << date << " " << time << "] ";
	string m;

    switch(m_logLevel) {
    case 1:
		ss << "ERROR: " << msg;
        break;
    case 2:
		ss << "FUNC: " << func << ", LINE: " << line << " ERROR: " << msg;
        break;
    case 3:
		ss << "FILE: " << file << "FUNC: " << func << ", LINE: " << line << " ERROR: " << msg;
        break;
    default:
        break;
    }

	m = ss.str();

	list<ILoggingStrategy *>::iterator it;
	for(it = m_logStrategies.begin(); it != m_logStrategies.end(); ++it)
		(*it)->log(m);
}

void SystemLogger::exportStatus(ITask *currTask)
{
	list<IStatusExportStrategy *>::iterator it;
    for(it = m_statusStrategies.begin(); it != m_statusStrategies.end(); ++it)
        (*it)->exportStatus(currTask);
}

void SystemLogger::exportDescription()
{
	list<IDescriptionExportStrategy *>::iterator it;
    for(it = m_descriptionStrategies.begin(); it != m_descriptionStrategies.end(); ++it)
        (*it)->exportDescription();
}

void SystemLogger::clearError()
{
    m_error = "no error";
}
