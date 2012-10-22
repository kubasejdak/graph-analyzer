#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase(DB_QT_DRIVER, "graph_analyzer_conn");
    m_db.setHostName(DB_HOST);
    m_db.setDatabaseName(DB_NAME);
    m_db.setUserName(DB_USER);
    m_db.setPassword(DB_PASS);

    m_lastError = "";

    if(!m_db.open())
        m_lastError = QString("couldn't set up a connection to db: %1").arg(m_db.lastError().text());
}

DatabaseManager::~DatabaseManager()
{
    m_db.close();
}

bool DatabaseManager::exec(QSqlQuery *query) {
    if(!query->exec()) {
        m_lastError = QString("error while executing query: %s").arg(query->lastError().databaseText());
        return false;
    }

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
