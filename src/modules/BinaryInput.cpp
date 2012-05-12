/*
 *  Filename	: BinaryInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "BinaryInput.h"

BinaryInput::BinaryInput() {
	id = getNextID();
	name = "BinaryInput";
	description = "Loads shellcode from binary files. Supports *.bin files.";
	extension = ".bin";
}

BinaryInput::~BinaryInput() {
}

bool BinaryInput::loadInput(string filename, ShellcodeSample *sample) {
	fstream file(filename.c_str(), fstream::in | fstream::binary);
	if(!file.is_open())
		return false;

	file.seekg(0, ios::end);
	int size = file.tellg();
	file.seekg(0, ios::beg);
	char *buffer = new char[size];

	file.read(buffer, size);
	file.close();

	sample->setCode((byte_t *) buffer);
	sample->getInfo().name = filename;
	sample->getInfo().size = size;

	return true;
}
