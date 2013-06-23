/*
 * Filename		: XMLParser.h
 * Author		: Kuba Sejdak
 * Created on	: 28-02-2013
 */

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <QtXml/QDomElement>

class QFile;
class QDomDocument;

class XMLParser {
public:
	XMLParser();
	virtual ~XMLParser();

	bool open(std::string filename);
	void close();

	void clear(std::string filename);

	bool hasRoot(std::string rootName);
	bool hasChild(QDomElement rootNode, std::string childNode);

	QDomElement root(std::string rootName);
	QDomElement child(QDomElement rootNode, std::string childNode);

	void removeRoot(QDomElement rootNode);
	void removeChild(QDomElement rootNode, QDomElement childNode);

	QDomElement createRoot(std::string rootName);
	QDomElement createChild(QDomElement rootNode, std::string childName);

private:
	void save();

	QFile *m_file;
	QDomDocument *m_doc;
	QDomElement m_docRoot;
};

#endif // XMLPARSER_H
