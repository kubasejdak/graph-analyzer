/*
 * Filename		: Options.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdint.h>
#include <string>
#include <QString>
#include <QFile>
#include <QtSql>
#include <QList>
#include <boost/program_options.hpp>
namespace opt = boost::program_options;
using namespace std;

#include <core/SystemLogger.h>

#define CONFIG_FILE         "/var/www/jsejdak/analyzer.conf"

class Options {
public:
	virtual ~Options() {}
	static Options *instance()
	{
		static Options obj;
		return &obj;
	}

	/* read options */
	bool readConfigFile();
	bool readDatabaseOptions();

	/* list options */
	void listOptions();

	/* database options (taken from config file */
	QString DB_QT_DRIVER;
	QString DB_HOST;
	QString DB_NAME;
	QString DB_USER;
	QString DB_PASS;
	QString ANA_TABLE_PREFIX;
	QString OPT_TABLE_PREFIX;
	QString ID_SEQ_POSTFIX;

	/* emulation options */
	int EMULATION_STEPS;
	int MAX_INPUT_FILE_SIZE;
	int RESEMBLENCE_LEVEL;
	bool SKIP_NONEXPLOIT_OUTPUT;
	bool PROTECT_AGAINST_BIG_FILES;
	bool SKIP_BROKEN_SAMPLES;
	int BROKEN_SAMPLE_SIZE;
	QString GRAPHS_DIR;

	/* pending files */
	QList<QString> pendingFiles;

private:
	Options();
};

#endif /* OPTIONS_H_ */
