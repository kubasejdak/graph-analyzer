/*
 *  Filename	: ShellcodeSample.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODESAMPLE_H_
#define SHELLCODESAMPLE_H_

/* standard headers */
#include <fstream>
using namespace std;

/* project headers */
#include <core/ShellcodeInfo.h>
#include <core/toolbox.h>

class ShellcodeSample {
public:
	ShellcodeSample();
	virtual ~ShellcodeSample();

	void extractCode();

private:
	fstream file;
	byte_t *code;
	ShellcodeInfo info;
};

#endif /* SHELLCODESAMPLE_H_ */
