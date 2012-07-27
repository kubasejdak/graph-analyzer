/*
 *  Filename	: BinaryInput.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "BinaryInput.h"

BinaryInput::BinaryInput() {
	id = getNextID();
	name = "BinaryInput";
	type = "binary";
	description = "Loads shellcode from binary files.";
}

BinaryInput::~BinaryInput() {
}

void BinaryInput::loadInput(string filename, queue<ShellcodeSample *> *samples) {
	fstream file(filename.c_str(), fstream::in | fstream::binary);
	if(!file.is_open())
		return;

	file.seekg(0, ios::end);
	int size = file.tellg();
	file.seekg(0, ios::beg);
	char *buffer = new char[size];

	file.read(buffer, size);
	file.close();

	ShellcodeSample *s = new ShellcodeSample();
	s->getInfo()->setName(filename);
	s->getInfo()->setFileType(type);
	s->getInfo()->setSize(size);
	s->setCode((byte_t *) buffer);
	samples->push(s);
}
