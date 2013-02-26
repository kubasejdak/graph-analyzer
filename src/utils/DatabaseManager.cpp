#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{
	m_db = QSqlDatabase::addDatabase(Options::instance()->DB_QT_DRIVER, "graph_analyzer_conn");
	m_db.setHostName(Options::instance()->DB_HOST);
	m_db.setDatabaseName(Options::instance()->DB_NAME);
	m_db.setUserName(Options::instance()->DB_USER);
	m_db.setPassword(Options::instance()->DB_PASS);

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
