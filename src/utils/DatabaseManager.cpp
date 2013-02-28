#include "DatabaseManager.h"

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
	bool stat = false;
	if(!init(CONFIG_FILE))
		return stat;

	/* parse root nodes */
	for(int i = 0; i < m_roots.size(); ++i) {
		QDomElement rootNode = m_roots.at(i).toElement();
		/* database options */
		if(rootNode.tagName() == "Database") {
			QDomNodeList options = rootNode.childNodes();
			stat = true;

			for(int i = 0; i < options.count(); ++i) {
				QDomElement e = options.at(i).toElement();
				QString node = e.tagName();

				if(node == "DBDriver")
					m_driver = e.attribute("val");
				else if(node == "DBHost")
					m_host = e.attribute("val");
				else if(node == "DBName")
					m_name = e.attribute("val");
				else if(node == "DBUser")
					m_user = e.attribute("val");
				else if(node == "DBPass")
					m_pass = e.attribute("val");
			}
		} /* CoreOptions */
	}

	return stat;
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
