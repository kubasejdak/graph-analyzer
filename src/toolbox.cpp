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

string hash(string data) {
	/* Fowler–Noll–Vo (FNV-1) hash function */
	int h = 2166136261U;
	for(unsigned i = 0; i < data.size(); ++i)
		h = (16777619U * h) ^ (uint8_t) data[i];

	if(h < 0)
		h = -h;
	return itos(h);
}
