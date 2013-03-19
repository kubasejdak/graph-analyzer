#include "DatabaseManager.h"
#include <core/Options.h>

DatabaseManager::DatabaseManager()
{
	/* initial configuration */
	m_driver = "QPSQL";
	m_host = "localhost";
	m_name = "graph_analyzer_db";
	m_user = "jsejdak";
	m_pass = "poliritiper";

	if(!readConfigXML())
		LOG_ERROR("failed to read XML configuration, using default options\n");

	listOptions();

	m_db = QSqlDatabase::addDatabase(m_driver, "graph_analyzer_conn");
	m_db.setHostName(m_host);
	m_db.setDatabaseName(m_name);
	m_db.setUserName(m_user);
	m_db.setPassword(m_pass);

    m_lastError = "";

    if(!m_db.open()) {
        m_lastError = QString("couldn't set up a connection to db: %1").arg(m_db.lastError().text());
        LOG_ERROR("canot set up a connection with database\n");
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
	m_driver = m_xmlParser.child(options, "DBDriver").attribute("val");
	m_host = m_xmlParser.child(options, "DBHost").attribute("val");
	m_name = m_xmlParser.child(options, "DBName").attribute("val");
	m_user = m_xmlParser.child(options, "DBUser").attribute("val");
	m_pass = m_xmlParser.child(options, "DBPass").attribute("val");

	m_xmlParser.close();
	LOG("SUCCESS\n\n");
	return true;
}

void DatabaseManager::listOptions()
{
	/* database settings */
	LOG("database settings:\n");
	LOG("m_driver: [%s]\n", m_driver.toStdString().c_str());
	LOG("m_host: [%s]\n", m_host.toStdString().c_str());
	LOG("m_name: [%s]\n", m_name.toStdString().c_str());
	LOG("m_user: [%s]\n", m_user.toStdString().c_str());
	LOG("m_pass: [%s]\n", m_pass.toStdString().c_str());
	LOG("SUCCESS\n\n");
}

bool DatabaseManager::exec(QSqlQuery *query) {
    if(!query->exec()) {
		m_lastError = QString("error while executing query: %1").arg(query->lastError().databaseText());
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("query: [%s]\n", query->executedQuery().toStdString().c_str());
        LOG_ERROR("executing query failed: [%s]\n", query->lastError().databaseText().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    return true;
}

bool DatabaseManager::clearTable(QString table)
{
	LOG("clearing table: [%s]\n", table.toStdString().c_str());

	/* delete all group assignments */
	QSqlQuery deleteQuery(DatabaseManager::instance()->database());
	QString q = QString("DELETE FROM %1").arg(table);
	deleteQuery.prepare(q);
	if(!DatabaseManager::instance()->exec(&deleteQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	return true;
}

int DatabaseManager::sequenceValue(QString table)
{
	LOG("getting next sequence value for table: [%s]\n", table.toStdString().c_str());

	/* get next sample_id number */
	QSqlQuery seqQuery(DatabaseManager::instance()->database());
	seqQuery.prepare("SELECT nextval(?)");
	seqQuery.addBindValue(table);
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
	idQuery.addBindValue(s->info()->graphName());
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

QString DatabaseManager::lastError()
{
    return m_lastError;
}
