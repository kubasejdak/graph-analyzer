/*
 * Filename		: Options.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <QString>
#include <QList>

#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>

#define CONFIG_FILE		"/var/www/jsejdak/GraphAnalyzer.conf"

class Options {
public:
	virtual ~Options() {}
	static Options *instance()
	{
		static Options m_instance;
		return &m_instance;
	}

	/* list options */
	void listOptions();
	void listPendingFiles();

	/* options */
	QString graphsDir;
    QString tmpPcapDir;
    QString tcpflowParams;
	int emulationSteps;
	bool skipBigFiles;
	int bigFileSize;
	bool skipBrokenSamples;
	int brokenSampleSize;

	/* pending files */
	QList<QString> pendingFiles;

private:
	Options();
	bool readConfigXML();

	XMLParser m_xmlParser;
};

#endif /* OPTIONS_H_ */
