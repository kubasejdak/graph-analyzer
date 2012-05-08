/*
 *  Filename	: BinaryInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "BinaryInput.h"

BinaryInput::BinaryInput() {
	id = getNextID();
	name = "BinaryInput";
	description = "Loads shellcode from binary files.";
}

BinaryInput::~BinaryInput() {
}

bool BinaryInput::loadInput(string filename) {
	return true;
}
