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

#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>
#include <utils/IOptionsReader.h>

class Options {
public:
	virtual ~Options() {}
	static Options *instance()
	{
		static Options m_instance;
		return &m_instance;
	}

	/* read options */
	bool readConfigXML();

	/* list options */
	void listOptions();
	void listPendingFiles();

	/* options */
	QString graphsDir;
	int emulationSteps;
	bool skipBigFiles;
	int bigFileSize;
	bool skipBrokenSamples;
	int brokenSampleSize;
	bool skipEmptySamples;
	int resemblenceLevel;

	/* pending files */
	QList<QString> pendingFiles;

private:
	Options();

	XMLParser m_xmlParser;
};

#endif /* OPTIONS_H_ */
