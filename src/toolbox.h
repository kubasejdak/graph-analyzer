/*
 * Filename	: toolbox.h
 * Author	: Kuba Sejdak
 * Created on	: 09-07-2012
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

/* standard headers */
#include <stdint.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
using namespace std;

typedef	uint8_t byte_t;

string itos(int value, bool hexadecimal = false);
string hash(string data);
bool isDirectory(string path);
bool directoryExists(string path);

#endif /* TOOLBOX_H_ */
