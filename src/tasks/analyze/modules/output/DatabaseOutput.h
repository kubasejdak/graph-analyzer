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

#include <tasks/analyze/modules/output/IOutput.h>
#include <core/ExploitInfo.h>
#include <core/Options.h>
#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/analyze/modules/ModuleManager.h>

class DatabaseOutput: public IOutput {
public:
	DatabaseOutput();
    virtual ~DatabaseOutput() {}

	virtual bool generateOutput(ExploitSample *sample);

private:
    bool checkDuplicate(ExploitInfo *info);
};

#endif /* DATABASEOUTPUT_H_ */
