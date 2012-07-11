/*
 *  Filename	: ShellcodeSample.h
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODESAMPLE_H_
#define SHELLCODESAMPLE_H_

/* project headers */
#include <core/ShellcodeInfo.h>
#include <core/Graph.h>
#include <toolbox.h>

class ShellcodeSample {
public:
	ShellcodeSample();
	virtual ~ShellcodeSample();

	byte_t *getCode();
	ShellcodeInfo *getInfo();
	Graph *getGraph();

	void setCode(byte_t *code);

private:
	byte_t *code;
	ShellcodeInfo *info;
	Graph *graph;
};

#endif /* SHELLCODESAMPLE_H_ */
