/*
 * Filename		: XMLParser.cpp
 * Author		: Kuba Sejdak
 * Created on	: 28-02-2013
 */

#include "XMLParser.h"

XMLParser::XMLParser()
{
	 m_doc = new QDomDocument("conf");
}

XMLParser::~XMLParser()
{
	delete m_doc;
}

bool XMLParser::init(QString filename)
{
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly))
		return false;
	
	if(!m_doc->setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	m_docElem = m_doc->documentElement();
	m_roots = m_docElem.childNodes();

	return true;
}
