/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#ifndef DATABASEOUTPUT_H_
#define DATABASEOUTPUT_H_

#include <QtCore>
#include <QtSql>
#include <QMap>
#include <QFile>

#include "AbstractOutput.h"
#include <core/SystemLogger.h>
#include <core/Options.h>
#include <core/DatabaseManager.h>
#include <modules/ModuleManager.h>

class DatabaseOutput: public AbstractOutput {
public:
	DatabaseOutput();
    virtual ~DatabaseOutput() {}

	virtual bool generateOutput(ShellcodeSample *sample);

	bool makeGroups(int resemblenceLevel);

private:
    void traitQuery(QSqlQuery *query, QString table, QMap<QString, QString> *v, int sample_id);
    bool checkDuplicate(ShellcodeInfo *info);

	/* group, resemblence rate */
	QMap<int, int> findMatchingGroups(int id, QVector<QString> loopHashes, int resemblenceLevel);
	void assignToGroup(int groupId, int memberId, int resemblenceRate);
	void activateUniqueGroups();
	bool isDoubleConnected(int group1, int group2);
};

#endif /* DATABASEOUTPUT_H_ */
