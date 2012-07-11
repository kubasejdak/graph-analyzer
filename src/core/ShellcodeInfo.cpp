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
	traits = new multimap<string, string>();
}

ShellcodeInfo::~ShellcodeInfo() {
}

void ShellcodeInfo::printInfo() {
	cout << "\t name : " << name << endl;
	cout << "\t size : " << size << endl;
	cout << "\t code offset : 0x" << hex << codeOffset << endl;
	cout << "\t shellcode present : " << (shellcodePresent ? "yes" : "no") << endl;
	multimap<string, string>::iterator it;
	for(it = traits->begin(); it != traits->end(); ++it)
		cout <<"\t " << (*it).first << " : " << (*it).second << endl;
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

multimap<string, string> *ShellcodeInfo::getTraits() {
	return traits;
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

void ShellcodeInfo::setTrait(string name, string value) {
	traits->insert(pair<string, string>(name, value));
}
