/*
 * Filename		: toolbox.cpp
 * Author		: Kuba Sejdak
 * Created on	: 20-07-2012
 */

#include "toolbox.h"

string itos(int value, bool hexadecimal)
{
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

string hash(string data)
{
	/* Fowler–Noll–Vo (FNV-1) hash function */
	int h = 2166136261U;
	for(unsigned i = 0; i < data.size(); ++i)
		h = (16777619U * h) ^ (uint8_t) data[i];

	if(h < 0)
		h = -h;
	return itos(h, true);
}

bool isDirectory(string path)
{
	struct stat sb;
	stat(path.c_str(), &sb);

	return S_ISDIR(sb.st_mode);
}

bool nameExists(string path)
{
	struct stat sb;
	int ret = stat(path.c_str(), &sb);

	return (ret == 0) ? true : false;
}

string extractBasename(string path)
{
	size_t pos = path.find_last_of("/");
	if(pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

string trimExtension(string path)
{
	size_t pos = path.find_last_of(".");
	if(pos == string::npos)
		return path;
	else
		return path.erase(pos);
}

bool isRelative(string path)
{
	if(path[0] == '/')
		return false;
	else
		return true;
}

string addSlash(string path)
{
	size_t pos = path.find_last_of("/");
	if(pos != path.size() - 1)
		path += "/";

	return path;
}
