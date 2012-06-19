/*
 *  Filename	: ShellcodeInfo.h
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODEINFO_H_
#define SHELLCODEINFO_H_

/* standard headers */
#include <list>
#include <string>
#include <stdint.h>
#include <iostream>
#include <iomanip>
using namespace std;

class ShellcodeInfo {
public:
	ShellcodeInfo();
	~ShellcodeInfo();

	void printInfo();

	string getName();
	int getSize();
	int getCodeOffset();
	bool isShellcodePresent();

	void setName(string name);
	void setSize(int size);
	void setCodeOffset(int codeOffset);
	void setShellcodePresent(bool shellcodePresent);
	void setModParam(string name, string value);

private:
	string name;
	int32_t size;
	int32_t codeOffset;
	bool shellcodePresent;
	list<pair<string, string> > modParams;
};

#endif /* SHELLCODEINFO_H_ */
