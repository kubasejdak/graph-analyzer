/*
 * Filename		: SystemLogger.h
 * Author		: Kuba Sejdak
 * Created on	: 29-07-2012
 */

#ifndef SYSTEMLOGGER_H_
#define SYSTEMLOGGER_H_

#include <cstdio>
#include <string>
#include <list>
#include <boost/current_function.hpp>

#include <utils/XMLParser.h>
#include <utils/Toolbox.h>

class ITask;
class ILoggingStrategy;
class IStatusExportStrategy;
class IDescriptionExportStrategy;

#define	LOG(fmt, args...)           SystemLogger::instance()->log(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, Toolbox::formatToString(fmt, ##args))
#define	LOG_ERROR(fmt, args...)     SystemLogger::instance()->logError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, Toolbox::formatToString(fmt, ##args))

class SystemLogger {
public:
	~SystemLogger();
    static SystemLogger *instance()
	{
		static SystemLogger m_instance;
		return &m_instance;
	}

    // error and status
	void setStatus(std::string status);
	std::string status();

	void setError(std::string error);
    void clearError();
	std::string error();
    int errorsNum();

    // logging
	void log(std::string file, std::string func, int line, std::string msg);
	void logError(std::string file, std::string func, int line, std::string msg);

    // exporting status
    void exportStatus(ITask *currTask);

    // export system description
    void exportDescription();

private:
	SystemLogger();
	bool readConfigXML();
	void listOptions();

	std::string m_status;
	std::string m_error;
    int m_allErrors;

    int m_logLevel;
	std::list<ILoggingStrategy *> m_logStrategies;
	std::list<IStatusExportStrategy *> m_statusStrategies;
	std::list<IDescriptionExportStrategy *> m_descriptionStrategies;

	XMLParser m_xmlParser;
};

#endif /* SYSTEMLOGGER_H_ */
