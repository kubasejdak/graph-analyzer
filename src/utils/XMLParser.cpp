/*
 * Filename		: XMLParser.cpp
 * Author		: Kuba Sejdak
 * Created on	: 28-02-2013
 */

#include "XMLParser.h"

#include <string>
#include <sys/file.h>
#include <QtXml/QDomDocument>
#include <QFile>

using namespace std;

XMLParser::XMLParser()
{
	 m_doc = NULL;
	 m_file = NULL;
}

XMLParser::~XMLParser()
{
	close();
}

bool XMLParser::open(string filename)
{
	m_file = new QFile(filename.c_str());
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

void XMLParser::clear(string filename)
{
	QFile file(filename.c_str());
    file.resize(0);
    file.open(QIODevice::WriteOnly);
    file.write("<xml/>");
    file.close();
}

bool XMLParser::hasRoot(string rootName)
{
	QDomNodeList roots = m_docRoot.childNodes();
	for(int i = 0; i < roots.size(); ++i) {
		if(roots.at(i).toElement().tagName() == rootName.c_str())
			return true;
	}

	return false;
}

bool XMLParser::hasChild(QDomElement rootNode, string childNode)
{
	QDomNodeList childList = rootNode.childNodes();
	for(int i = 0; i < childList.size(); ++i) {
		if(childList.at(i).toElement().tagName() == childNode.c_str())
			return true;
	}

	return false;
}

QDomElement XMLParser::root(string rootName)
{
	QDomNodeList roots = m_docRoot.childNodes();
	for(int i = 0; i < roots.size(); ++i) {
		if(roots.at(i).toElement().tagName() == rootName.c_str())
			return roots.at(i).toElement();
	}

	return QDomElement();
}

QDomElement XMLParser::child(QDomElement rootNode, string childNode)
{
	QDomNodeList childList = rootNode.childNodes();
	for(int i = 0; i < childList.size(); ++i) {
		if(childList.at(i).toElement().tagName() == childNode.c_str())
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

QDomElement XMLParser::createRoot(string rootName)
{
	QDomElement newRoot = m_doc->createElement(rootName.c_str());
	m_docRoot.appendChild(newRoot);
    return newRoot;
}

QDomElement XMLParser::createChild(QDomElement rootNode, string childName)
{
	QDomElement newChild = m_doc->createElement(childName.c_str());
	rootNode.appendChild(newChild);
    return newChild;
}

void XMLParser::save()
{
	m_file->resize(0);
	m_file->write(m_doc->toByteArray(4));
}
