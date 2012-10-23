#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <QString>
#include <QMap>
#include <QtSql>

#include <core/Options.h>
#include <core/SystemLogger.h>

class DatabaseManager {
public:
    virtual ~DatabaseManager();
    static DatabaseManager *instance()
    {
        static DatabaseManager obj;
        return &obj;
    }

    bool exec(QSqlQuery *query);
    QSqlDatabase &database();
    QString lastError();

private:
    DatabaseManager();

    QSqlDatabase m_db;
    QString m_lastError;
};

#endif /* DATABASEMANAGER_H_ */
