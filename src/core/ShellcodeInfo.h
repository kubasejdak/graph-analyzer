/*
 *  Filename	: ShellcodeInfo.h
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODEINFO_H_
#define SHELLCODEINFO_H_

/* standard headers */
#include <map>
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
	string getFileType();
	int getSize();
	int getCodeOffset();
	bool isShellcodePresent();
	multimap<string, string> *getTraits();

	void setName(string name);
	void setFileType(string fileType);
	void setSize(int size);
	void setCodeOffset(int codeOffset);
	void setShellcodePresent(bool shellcodePresent);
	void setTrait(string name, string value);

private:
	string name;
	string fileType;
	int32_t size;
	int32_t codeOffset;
	bool shellcodePresent;
	multimap<string, string> *traits;
};

#endif /* SHELLCODEINFO_H_ */
