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

class XMLParser {
public:
	XMLParser();
	virtual ~XMLParser();

	bool open(QString filename);
	void close();

    void clear(QString filename);

	bool hasRoot(QString rootName);
	bool hasChild(QDomElement rootNode, QString childNode);

	QDomElement root(QString rootName);
	QDomElement child(QDomElement rootNode, QString childNode);

	void removeRoot(QDomElement rootNode);
	void removeChild(QDomElement rootNode, QDomElement childNode);

	QDomElement createRoot(QString rootName);
	QDomElement createChild(QDomElement rootNode, QString childName);

private:
	void save();

	QFile *m_file;
	QDomDocument *m_doc;
	QDomElement m_docRoot;
};

#endif // XMLPARSER_H
