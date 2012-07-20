/*
 * Filename	: DecoderKey.cpp
 * Author	: Kuba Sejdak
 * Created on	: 12-07-2012
 */

/* debug */
#define LOCAL_DEBUG
#include <debug.h>

#include "DecoderKey.h"

DecoderKey::DecoderKey() {
	id = getNextID();
	name = "DecoderKey";
	description = "Searches for decoding block and extracts XOR key.";
}

DecoderKey::~DecoderKey() {
}

bool DecoderKey::perform(ShellcodeSample *sample) {
	return true;
}
