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

private:
    void traitQuery(QSqlQuery *query, QString table, QMap<QString, QString> *v, int sample_id);
    bool checkDuplicate(ShellcodeInfo *info);
};

#endif /* DATABASEOUTPUT_H_ */
