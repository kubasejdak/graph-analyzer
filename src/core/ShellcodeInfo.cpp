/*
 *  Filename	: ShellcodeInfo.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 09-05-2012
 */

#include "ShellcodeInfo.h"

ShellcodeInfo::ShellcodeInfo()
{
    m_name = "";
    m_extractedFrom = "";
    m_graphName = "";
    m_fileType = "unknown";
    m_size = 0;
    m_codeOffset = 0;
    m_shellcodePresent = false;
    m_traits = new QMultiMap<QString, QMap<QString, QString> *> ();
    m_querys = new QList<QSqlQuery> ();
}

ShellcodeInfo::~ShellcodeInfo()
{
    QMultiMap<QString, QMap<QString, QString> *>::iterator it_map;
    for(it_map = m_traits->begin(); it_map != m_traits->end(); ++it_map)
        delete it_map.value();

    m_traits->clear();
    delete m_traits;

    m_querys->clear();
    delete m_querys;
}

void ShellcodeInfo::printInfo()
{
    cout << "\t name : " << m_name.toStdString() << endl;
    cout << "\t extracted from: " << m_extractedFrom.toStdString() << endl;
    cout << "\t graph name: " << m_graphName.toStdString() << endl;
    cout << "\t file type : " << m_fileType.toStdString() << endl;
    cout << "\t file size : " << dec << m_size << endl;
    cout << "\t shellcode present : " << (m_shellcodePresent ? "yes" : "no") << endl;
    if(m_shellcodePresent)
        cout << "\t shellcode offset : 0x" << hex << m_codeOffset << endl;
    QMultiMap<QString, QMap<QString, QString> *>::iterator it;
    QMap<QString, QString>::iterator it2;
    for(it = m_traits->begin(); it != m_traits->end(); ++it) {
        cout << "\t * " << it.key().toStdString() << " *" << "\t";
        for(it2 = it.value()->begin(); it2 != it.value()->end(); ++it2)
            cout << it2.key().toStdString() << " : " << it2.value().toStdString() << ", ";
		cout << endl << endl;
	}
}

QString ShellcodeInfo::name()
{
    return m_name;
}

QString ShellcodeInfo::extractedFrom()
{
    return m_extractedFrom;
}

QString ShellcodeInfo::graphName()
{
    return m_graphName;
}

QString ShellcodeInfo::fileType()
{
    return m_fileType;
}

int32_t ShellcodeInfo::size()
{
    return m_size;
}

int32_t ShellcodeInfo::codeOffset()
{
    return m_codeOffset;
}

bool ShellcodeInfo::isShellcodePresent()
{
    return m_shellcodePresent;
}

QMultiMap<QString, QMap<QString, QString> *> *ShellcodeInfo::traits()
{
    return m_traits;
}

QList<QSqlQuery> *ShellcodeInfo::querys()
{
    return m_querys;
}

void ShellcodeInfo::setName(QString name)
{
    m_name = name;
}

void ShellcodeInfo::setExtractedFrom(QString name)
{
    m_extractedFrom = name;
}

void ShellcodeInfo::setGraphName(QString name)
{
    m_graphName = name;
}

void ShellcodeInfo::setFileType(QString fileType)
{
    m_fileType = fileType;
}

void ShellcodeInfo::setSize(int size)
{
    m_size = size;
}

void ShellcodeInfo::setCodeOffset(int codeOffset)
{
    m_codeOffset = codeOffset;
}

void ShellcodeInfo::setShellcodePresent(bool shellcodePresent)
{
    m_shellcodePresent = shellcodePresent;
}

void ShellcodeInfo::setTrait(QString name, QMap<QString, QString> *value)
{
	/* ensure that key/value is unique */
    if(m_traits->contains(name, value))
        return;

    m_traits->insert(name, value);
}

void ShellcodeInfo::setQuery(QSqlQuery q)
{
    m_querys->push_back(q);
}
