/*
 *  Filename	: ShellcodeInfo.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 09-05-2012
 */

#include "ShellcodeInfo.h"

ShellcodeInfo::ShellcodeInfo() {
	name = "";
	fileType = "unknown";
	size = 0;
	codeOffset = 0;
	shellcodePresent = false;
	traits = new multimap<string, map<string, string> *>();
}

ShellcodeInfo::~ShellcodeInfo() {
	multimap<string, map<string, string> *>::iterator it;
	for(it = traits->begin(); it != traits->end(); ++it)
		delete (*it).second;

	delete traits;
}

void ShellcodeInfo::printInfo() {
	cout << "\t name : " << name << endl;
	cout << "\t file type : " << fileType << endl;
	cout << "\t size : " << dec <<size << endl;
	cout << "\t shellcode present : " << (shellcodePresent ? "yes" : "no") << endl;
	cout << "\t shellcode offset : 0x" << hex << codeOffset << endl;
	multimap<string, map<string, string> *>::iterator it;
	map<string, string>::iterator it2;
	for(it = traits->begin(); it != traits->end(); ++it) {
		cout <<"\t * " << (*it).first << " *" << "\t";
		for(it2 = (*it).second->begin(); it2 != (*it).second->end(); ++it2)
			cout << (*it2).first << " : " << (*it2).second << ", ";
		cout << endl << endl;
	}
}

string ShellcodeInfo::getName() {
	return name;
}

string ShellcodeInfo::getFileType() {
	return fileType;
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

multimap<string, map<string, string> *> *ShellcodeInfo::getTraits() {
	return traits;
}

void ShellcodeInfo::setName(string name) {
	this->name = name;
}

void ShellcodeInfo::setFileType(string fileType) {
	this->fileType = fileType;
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

void ShellcodeInfo::setTrait(string name, map<string, string>  *value) {
	/* ensure that key/value is unique */
	pair<multimap<string, map<string, string> *>::iterator, multimap<string, map<string, string> *>::iterator> ret;
	multimap<string, map<string, string> *>::iterator it;
	ret = traits->equal_range(name);
	for(it = ret.first; it != ret.second; ++it) {
		if(equalMaps((*it).second, value))
			return;
	}

	traits->insert(pair<string, map<string, string> *>(name, value));
}

bool ShellcodeInfo::equalMaps(map<string, string> *a, map<string, string> *b) {
	if(a->size() != b->size())
		return false;

	map<string, string>::iterator it;
	for(it = a->begin(); it != a->end(); ++it) {
		if((*b)[(*it).first] != (*it).second)
			return false;
	}

	return true;
}
