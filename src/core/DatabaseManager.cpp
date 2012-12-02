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
        m_lastError = QString("error while executing query: %s").arg(query->lastError().databaseText());
        LOG_ERROR("executing query failed: [%s]\n", query->lastError().databaseText().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    LOG("executed query: [%s]\n", query->executedQuery().toStdString().c_str());
    LOG("SUCCESS\n\n");
    return true;
}

QSqlDatabase &DatabaseManager::database()
{
    return m_db;
}

QString DatabaseManager::lastError()
{
    return m_lastError;
}
