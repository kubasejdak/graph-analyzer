/*
 *  Filename	: DatabaseManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "DatabaseManager.h"

#include <string>
#include <sstream>
#include <QtSql>

#include <core/Options.h>
#include <core/ExploitSample.h>
#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>

using namespace std;

DatabaseManager::DatabaseManager()
{
	// initial configuration
	m_driver = "QPSQL";
	m_host = "localhost";
	m_name = "graph_analyzer_db";
	m_user = "jsejdak";
	m_pass = "poliritiper";

	if(!readConfigXML())
		LOG_ERROR("failed to read XML configuration, using default options\n");

	listOptions();

    m_db = QSqlDatabase::addDatabase(m_driver.c_str(), "graph_analyzer_conn");
    m_db.setHostName(m_host.c_str());
    m_db.setDatabaseName(m_name.c_str());
    m_db.setUserName(m_user.c_str());
    m_db.setPassword(m_pass.c_str());

    m_lastError = "";

    if(!m_db.open()) {
        stringstream ss;
        ss << "couldn't set up a connection to db: " << m_db.lastError().text().toStdString();
        m_lastError = ss.str();
        LOG_ERROR("canot set up a connection with database, [%s]\n", ss.str().c_str());
    }

    LOG("created connection to database\n");
    LOG("SUCCESS\n\n");
}

DatabaseManager::~DatabaseManager()
{
    m_db.close();
}

bool DatabaseManager::readConfigXML()
{
	if(!m_xmlParser.open(CONFIG_FILE))
		return false;

	if(!m_xmlParser.hasRoot("Database")) {
		m_xmlParser.close();
		return false;
	}

	QDomElement options = m_xmlParser.root("Database");
    m_driver = m_xmlParser.child(options, "DBDriver").attribute("val").toStdString();
    m_host = m_xmlParser.child(options, "DBHost").attribute("val").toStdString();
    m_name = m_xmlParser.child(options, "DBName").attribute("val").toStdString();
    m_user = m_xmlParser.child(options, "DBUser").attribute("val").toStdString();
    m_pass = m_xmlParser.child(options, "DBPass").attribute("val").toStdString();

	m_xmlParser.close();
	LOG("SUCCESS\n\n");
	return true;
}

void DatabaseManager::listOptions()
{
	// database settings
	LOG("database settings:\n");
    LOG("m_driver: [%s]\n", m_driver.c_str());
    LOG("m_host: [%s]\n", m_host.c_str());
    LOG("m_name: [%s]\n", m_name.c_str());
    LOG("m_user: [%s]\n", m_user.c_str());
    LOG("m_pass: [%s]\n", m_pass.c_str());
	LOG("SUCCESS\n\n");
}

bool DatabaseManager::exec(QSqlQuery *query) {
	if(query->exec() == false) {
        stringstream ss;
        ss << "error while executing query: " << query->lastError().databaseText().toStdString();
        m_lastError = ss.str();
        SystemLogger::instance()->setError(m_lastError);
		LOG_ERROR("query: [%s]\n", query->executedQuery().toStdString().c_str());
        LOG_ERROR("executing query failed: [%s]\n", m_lastError.c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    return true;
}

bool DatabaseManager::clearTable(string table)
{
    LOG("clearing table: [%s]\n", table.c_str());

	// delete all group assignments
	QSqlQuery deleteQuery(DatabaseManager::instance()->database());
    deleteQuery.prepare("DELETE FROM ?");
    deleteQuery.addBindValue(table.c_str());
	if(!DatabaseManager::instance()->exec(&deleteQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	return true;
}

int DatabaseManager::sequenceValue(string table)
{
    LOG("getting next sequence value for table: [%s]\n", table.c_str());

	// get next sample_id number
	QSqlQuery seqQuery(DatabaseManager::instance()->database());
	seqQuery.prepare("SELECT nextval(?)");
    seqQuery.addBindValue(table.c_str());
	if(!DatabaseManager::instance()->exec(&seqQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return -1;
	}
	seqQuery.next();
	int seqValue = seqQuery.record().value("nextval").toInt();

	return seqValue;
}

int DatabaseManager::sampleId(ExploitSample *s)
{
	QSqlQuery idQuery(DatabaseManager::instance()->database());
	idQuery.prepare("SELECT * FROM analyze_sample WHERE graph_name = ?");
	idQuery.addBindValue(s->info()->graphName().c_str());
	if(!DatabaseManager::instance()->exec(&idQuery))
		return -1;

	idQuery.next();
	int id = idQuery.record().value("id").toInt();
	return id;
}

int DatabaseManager::groupId(ExploitSample *s)
{
	QSqlQuery idQuery(DatabaseManager::instance()->database());
	idQuery.prepare("SELECT * FROM analyze_samplegroup WHERE leader_id = ?");
	idQuery.addBindValue(sampleId(s));
	if(!DatabaseManager::instance()->exec(&idQuery))
		return -1;

	idQuery.next();
	int id = idQuery.record().value("id").toInt();
	return id;
}

QSqlDatabase &DatabaseManager::database()
{
    return m_db;
}

string DatabaseManager::lastError()
{
    return m_lastError;
}
