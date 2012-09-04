/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#ifndef DATABASEOUTPUT_H_
#define DATABASEOUTPUT_H_

/* standard headers */
#include <QtCore>
#include <QtSql>
#include <map>
using namespace std;

/* project headers */
#include "AbstractOutput.h"
#include <options.h>
#include <modules/ModuleManager.h>

class DatabaseOutput: public AbstractOutput {
public:
	DatabaseOutput();
	virtual ~DatabaseOutput();

	virtual bool generateOutput(ShellcodeSample *sample);

private:
	QSqlQuery traitQuery(string table, map<string, string> *v, int sample_id);

	QSqlDatabase db;
	map<int, AbstractOutput *> *analyzeMods;
};

#endif /* DATABASEOUTPUT_H_ */
