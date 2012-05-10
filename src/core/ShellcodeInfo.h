/*
 *  Filename	: ShellcodeInfo.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODEINFO_H_
#define SHELLCODEINFO_H_

enum GeneralType {
	UNKNOWN,
	REMOTE,
	LOCAL
};

enum ConnType {
	UNKNOWN_CONN
};

/* standard headers */
#include <list>
#include <string>
using namespace std;

class ShellcodeInfo {
public:
	ShellcodeInfo();
	~ShellcodeInfo();

	string name;
	int size;
	int codeOffset;
	bool shellcodePresent;
	GeneralType genType;
	ConnType connType;
	list<string> usedSysAPI;
};

#endif /* SHELLCODEINFO_H_ */
