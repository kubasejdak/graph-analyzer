/*
 *  Filename	: ShellcodeInfo.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 09-05-2012
 */

#include "ShellcodeInfo.h"

ShellcodeInfo::ShellcodeInfo() {
	name = "";
	size = 0;
	codeOffset = 0;
	shellcodePresent = false;
	modParams.clear();
}

ShellcodeInfo::~ShellcodeInfo() {
}

void ShellcodeInfo::printInfo() {
	cout << "name = " << name << endl;
	cout << "size = " << size << endl;
	cout << "code offset = 0x" << hex << codeOffset << endl;
	cout << "shellcode present = " << (shellcodePresent ? "yes" : "no") << endl;
	map<string, string>::iterator it;
	for(it = modParams.begin(); it != modParams.end(); ++it)
		cout << (*it).first << " = " << (*it).second << endl;
}

string ShellcodeInfo::getName() {
	return name;
}

int32_t ShellcodeInfo::getSize() {
	return size;
}

int32_t ShellcodeInfo::getCodeOffset() {
	return codeOffset;
}

bool ShellcodeInfo::isShellcodePresent() {
	return shellcodePresent;
}

void ShellcodeInfo::setName(string name) {
	this->name = name;
}

void ShellcodeInfo::setSize(int size) {
	this->size = size;
}

void ShellcodeInfo::setCodeOffset(int codeOffset) {
	this->codeOffset = codeOffset;
}

void ShellcodeInfo::setShellcodePresent(bool shellcodePresent) {
	this->shellcodePresent = shellcodePresent;
}

void ShellcodeInfo::setModParam(string name, string value) {
	modParams[name] = value;
}
