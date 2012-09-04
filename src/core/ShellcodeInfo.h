/*
 *  Filename	: ShellcodeInfo.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODEINFO_H_
#define SHELLCODEINFO_H_

/* standard headers */
#include <map>
#include <list>
#include <string>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <QtSql>
using namespace std;

class ShellcodeInfo {
public:
	ShellcodeInfo();
	~ShellcodeInfo();

	void printInfo();

	string getName();
	string getExtractedFrom();
	string getGraphName();
	string getFileType();
	int getSize();
	int getCodeOffset();
	bool isShellcodePresent();
	multimap<string, map<string, string> *> *getTraits();
	list<QSqlQuery> *getQuerys();

	void setName(string name);
	void setExtractedFrom(string name);
	void setGraphName(string name);
	void setFileType(string fileType);
	void setSize(int size);
	void setCodeOffset(int codeOffset);
	void setShellcodePresent(bool shellcodePresent);
	void setTrait(string name, map<string, string> *value);
	void setQuery(QSqlQuery q);

private:
	bool equalMaps(map<string, string> *a, map<string, string> *b);
	string name;
	string extractedFrom;
	string graphName;
	string fileType;
	int32_t size;
	int32_t codeOffset;
	bool shellcodePresent;
	multimap<string, map<string, string> *> *traits;
	list<QSqlQuery> *querys;
};

#endif /* SHELLCODEINFO_H_ */
