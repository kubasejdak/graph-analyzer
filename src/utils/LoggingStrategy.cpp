/*
 *  Filename	: LoggingStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "LoggingStrategy.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

FileLoggingStrategy::FileLoggingStrategy(string filename) : m_logFile(filename)
{
}

void FileLoggingStrategy::log(string message)
{
	if(m_logFile != "") {
		ofstream file(m_logFile, ios_base::out | ios_base::app);
		if(file.is_open() == false)
			return;

		// if size is greater than 1GB
		file.seekp(0, ios_base::end);
		int size = file.tellp();
		if(size > 1024*1024*1024) {
			file.close();
			file.open(m_logFile, ios_base::out | ios_base::trunc);
		}


		file.write(message.c_str(), message.size());
		file.close();
	}
}

string FileLoggingStrategy::description()
{
	stringstream ss;
	ss << "file: " << m_logFile;
	return ss.str();
}

void ConsoleLoggingStrategy::log(string message)
{
	cout << message;
}

string ConsoleLoggingStrategy::description()
{
	return "console";
}
