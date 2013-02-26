#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <QString>
#include <QMap>
#include <QtSql>

#include <core/Options.h>
#include <utils/SystemLogger.h>
#include <core/ExploitSample.h>

class DatabaseManager {
public:
    virtual ~DatabaseManager();
    static DatabaseManager *instance()
    {
		static DatabaseManager m_instance;
		return &m_instance;
    }

	/* general */
    bool exec(QSqlQuery *query);
	bool clearTable(QString table);
	int sequenceValue(QString table);
	int sampleId(ExploitSample *s);
	int groupId(ExploitSample *s);

	/* utility */
    QSqlDatabase &database();
    QString lastError();

private:
    DatabaseManager();

    QSqlDatabase m_db;
    QString m_lastError;
};

#endif /* DATABASEMANAGER_H_ */
