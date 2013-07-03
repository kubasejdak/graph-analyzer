/*
 * Filename		: Options.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

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

	// list options
	void listOptions();
	void listPendingFiles();

	// options
	std::string graphsDir;
	std::string tmpPcapDir;
	std::string tcpflowParams;
	int emulationSteps;
	bool skipBigFiles;
	unsigned long bigFileSize;
	bool skipBrokenSamples;
	int brokenSampleSize;

private:
	Options();
	bool readConfigXML();

	XMLParser m_xmlParser;
};

#endif /* OPTIONS_H_ */
