/*
 * Filename		: XMLParser.h
 * Author		: Kuba Sejdak
 * Created on	: 28-02-2013
 */

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QFile>

class XMLParser
{
public:
	XMLParser();
	virtual ~XMLParser();

protected:
	bool init(QString file);

	 QDomDocument *m_doc;
	 QDomElement m_docElem;
	 QDomNodeList m_roots;
};

#endif // XMLPARSER_H
