/*
 *  Filename	: ShellcodeSample.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODESAMPLE_H_
#define SHELLCODESAMPLE_H_

/* project headers */
#include <core/ShellcodeInfo.h>
#include <core/toolbox.h>

class ShellcodeSample {
public:
	ShellcodeSample();
	virtual ~ShellcodeSample();

	byte_t * getCode();
	ShellcodeInfo getInfo();

	void setCode(byte_t *code);

private:
	byte_t *code;
	ShellcodeInfo info;
};

#endif /* SHELLCODESAMPLE_H_ */
