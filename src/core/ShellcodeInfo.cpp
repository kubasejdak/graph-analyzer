/*
 *  Filename	: ShellcodeInfo.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 09-05-2012
 */

#include "ShellcodeInfo.h"

ShellcodeInfo::ShellcodeInfo() {
	size = 0;
	codeOffset = 0;
	shellcodePresent = false;
	genType = UNKNOWN;
	connType = UNKNOWN_CONN;
	usedSysAPI.clear();
}

ShellcodeInfo::~ShellcodeInfo() {
}
