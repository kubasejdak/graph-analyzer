/*
 * Filename		: XMLParser.cpp
 * Author		: Kuba Sejdak
 * Created on	: 28-02-2013
 */

#include "XMLParser.h"

#include <sys/file.h>

XMLParser::XMLParser()
{
	 m_doc = NULL;
	 m_file = NULL;
}

XMLParser::~XMLParser()
{
	close();
}

bool XMLParser::open(QString filename)
{
	m_file = new QFile(filename);
	if(m_file->open(QIODevice::ReadWrite) == false) {
		delete m_file;
		m_file = NULL;
		return false;
	}

	m_doc = new QDomDocument("conf");
	if(m_doc->setContent(m_file) == false) {
		close();
		return false;
	}

	m_docRoot = m_doc->documentElement();
	return true;
}

void XMLParser::close()
{
	if(m_file) {
		save();
		m_file->close();
		delete m_file;
		m_file = NULL;
	}

	if(m_doc) {
		delete m_doc;
		m_doc = NULL;
	}
}

void XMLParser::clear(QString filename)
{
    QFile file(filename);
    file.resize(0);
    file.open(QIODevice::WriteOnly);
    file.write("<xml/>");
    file.close();
}

bool XMLParser::hasRoot(QString rootName)
{
	QDomNodeList roots = m_docRoot.childNodes();
	for(int i = 0; i < roots.size(); ++i) {
		if(roots.at(i).toElement().tagName() == rootName)
			return true;
	}

	return false;
}

bool XMLParser::hasChild(QDomElement rootNode, QString childNode)
{
	QDomNodeList childList = rootNode.childNodes();
	for(int i = 0; i < childList.size(); ++i) {
		if(childList.at(i).toElement().tagName() == childNode)
			return true;
	}

	return false;
}

QDomElement XMLParser::root(QString rootName)
{
	QDomNodeList roots = m_docRoot.childNodes();
	for(int i = 0; i < roots.size(); ++i) {
		if(roots.at(i).toElement().tagName() == rootName)
			return roots.at(i).toElement();
	}

	return QDomElement();
}

QDomElement XMLParser::child(QDomElement rootNode, QString childNode)
{
	QDomNodeList childList = rootNode.childNodes();
	for(int i = 0; i < childList.size(); ++i) {
		if(childList.at(i).toElement().tagName() == childNode)
			return childList.at(i).toElement();
	}

	return QDomElement();
}

void XMLParser::removeRoot(QDomElement rootNode)
{
    m_docRoot.removeChild(rootNode);
}

void XMLParser::removeChild(QDomElement rootNode, QDomElement childNode)
{
    rootNode.removeChild(childNode);
}

QDomElement XMLParser::createRoot(QString rootName)
{
	QDomElement newRoot = m_doc->createElement(rootName);
	m_docRoot.appendChild(newRoot);
    return newRoot;
}

QDomElement XMLParser::createChild(QDomElement rootNode, QString childName)
{
	QDomElement newChild = m_doc->createElement(childName);
	rootNode.appendChild(newChild);
    return newChild;
}

void XMLParser::save()
{
	m_file->resize(0);
	m_file->write(m_doc->toByteArray(4));
}
