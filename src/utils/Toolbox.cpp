/*
 * Filename		: Toolbox.cpp
 * Author		: Kuba Sejdak
 * Created on	: 20-07-2012
 */

#include "Toolbox.h"

#include <sstream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <QFileInfo>
#include <QDir>

using namespace std;

string Toolbox::itos(int n, int base)
{
	char buffer[1024];
	switch(base) {
	case 8:
		sprintf(buffer, "%o", n);
		break;
	case 10:
		sprintf(buffer, "%d", n);
		break;
	case 16:
		sprintf(buffer, "%#x", n);
		break;
	default:
		break;
	}

	return string(buffer);
}

string Toolbox::hash(string data)
{
	// Fowler–Noll–Vo (FNV-1) hash function
	string str = data;
	int h = 2166136261U;
    for(unsigned i = 0; i < str.size(); ++i)
        h = (16777619U * h) ^ (uint8_t) str[i];

	if(h < 0)
		h = -h;
    return itos(h, 16);
}

string Toolbox::makeRelative(string path)
{
	QFileInfo f(path.c_str());
	return f.absoluteFilePath().toStdString();
}

string Toolbox::removeSlash(string path)
{
	if(path.back() == '/')
		path.substr(0, path.size() - 1);

	return path;
}

string Toolbox::pcapFlowBasename(string path)
{
	int n = path.find_last_of("/");
	return path.substr(n + 1);
}

bool Toolbox::removeDirectory(string path)
{
	return QDir().rmdir(path.c_str());
}

string Toolbox::formatToString(const string fmt...) {
	int size = 100;
	string str;
	while(1) {
		str.resize(size);
		va_list argptr;
		va_start(argptr, fmt);
		int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), argptr);
		va_end(argptr);
		if(n > -1 && n < size) {
			str.resize(n);
			return str;
		}
		if(n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}
