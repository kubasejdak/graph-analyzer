#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <QString>
#include <QMap>
#include <QtSql>

#include <core/Options.h>
#include <core/ExploitSample.h>
#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>

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
	bool readConfigXML();
	void listOptions();

    QSqlDatabase m_db;
    QString m_lastError;

	QString m_driver;
	QString m_host;
	QString m_name;
	QString m_user;
	QString m_pass;

	XMLParser m_xmlParser;
};

#endif /* DATABASEMANAGER_H_ */
