/*
 * Filename	: toolbox.cpp
 * Author	: Kuba Sejdak
 * Created on	: 20-07-2012
 */

#include "toolbox.h"

string itos(int value, bool hexadecimal) {
	stringstream ss;
	string s = "";
	if(hexadecimal) {
		s = "0x";
		ss << hex << value;
		s += ss.str();
	}
	else {
		ss << value;
		s += ss.str();
	}

	return s;
}
