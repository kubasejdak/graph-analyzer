/*
 *  Filename	: DatabaseManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <string>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <utils/XMLParser.h>

class ExploitSample;

class DatabaseManager {
public:
    virtual ~DatabaseManager();
    static DatabaseManager *instance()
    {
		static DatabaseManager m_instance;
		return &m_instance;
    }

	// general
    bool exec(QSqlQuery *query);
	bool clearTable(std::string table);
	int sequenceValue(std::string table);
	int sampleId(ExploitSample *s);
	int groupId(ExploitSample *s);

	// utility
    QSqlDatabase &database();
	std::string lastError();

private:
    DatabaseManager();
	bool readConfigXML();
	void listOptions();

    QSqlDatabase m_db;
	std::string m_lastError;

	std::string m_driver;
	std::string m_host;
	std::string m_name;
	std::string m_user;
	std::string m_pass;

	XMLParser m_xmlParser;
};

#endif /* DATABASEMANAGER_H_ */
