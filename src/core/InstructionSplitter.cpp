/*
 * Filename	: InstructionSplitter.cpp
 * Author	: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#include "InstructionSplitter.h"

InstructionSplitter::InstructionSplitter() {
}

InstructionSplitter::~InstructionSplitter() {
}

string InstructionSplitter::getAddress() {
	if(holder.size() < 1)
		return "";

	return holder[0];
}

string InstructionSplitter::getCode() {
	if(holder.size() < 2)
		return "";

	return holder[1];
}

string InstructionSplitter::getSyscall() {
	if(holder.size() != 2)
		return "";

	return holder[1];
}

string InstructionSplitter::getInstr() {
	if(holder.size() < 3)
		return "";

	return holder[2];
}

string InstructionSplitter::getFirstArg() {
	if(holder.size() < 4)
		return "";

	return holder[3];
}

string InstructionSplitter::getSecondArg() {
	if(holder.size() < 5)
		return "";

	return holder[4];
}

InstructionSplitter &InstructionSplitter::operator=(string s) {
	holder.clear();
	s.erase(s.size() - 2, 2);
	split(holder, s, is_any_of(" ,"), token_compress_on);

	return *this;
}
