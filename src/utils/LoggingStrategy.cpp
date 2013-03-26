/*
 *  Filename	: LoggingStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "LoggingStrategy.h"

#include <QFile>
#include <iostream>
using namespace std;

FileLoggingStrategy::FileLoggingStrategy(QString filename) : m_logFile(filename)
{
}

void FileLoggingStrategy::log(QString message)
{
	if(m_logFile != "") {
		QFile f(m_logFile);
		f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
		if(!f.isOpen())
			return;

		/* if size is greater than 1GB */
		if(f.size() > 1024*1024*1024)
			f.resize(0);

		f.write(message.toUtf8());
		f.close();
	}
}

QString FileLoggingStrategy::description()
{
	return QString("file: %1").arg(m_logFile);
}

void ConsoleLoggingStrategy::log(QString message)
{
	cout << message.toStdString();
}

QString ConsoleLoggingStrategy::description()
{
	return "console";
}
