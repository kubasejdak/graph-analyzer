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
using namespace std;

typedef	uint8_t byte_t;

string itos(int value, bool hexadecimal = false);

#endif /* TOOLBOX_H_ */
