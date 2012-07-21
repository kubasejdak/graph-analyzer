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
	extension = ".bin";
	description = "Loads shellcode from binary files. Supports " + extension + " files.";
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
	sample->getInfo()->setName(filename);
	sample->getInfo()->setFileType("BIN (plain x86 mashine code)");
	sample->getInfo()->setSize(size);

	return true;
}
