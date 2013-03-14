#ifndef IOPTIONSREADER_H
#define IOPTIONSREADER_H

#include "XMLParser.h"

#define CONFIG_FILE		"/var/www/jsejdak/GraphAnalyzer.conf"

class IOptionsReader {
public:
	IOptionsReader() {}
	virtual ~IOptionsReader() {}

protected:
	virtual bool readConfigXML() = 0;

	XMLParser m_xmlParser;
};

#endif // IOPTIONSREADER_H
