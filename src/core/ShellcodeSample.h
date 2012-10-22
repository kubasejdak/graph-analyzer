/*
 *  Filename	: ShellcodeSample.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef SHELLCODESAMPLE_H_
#define SHELLCODESAMPLE_H_

#include <core/ShellcodeInfo.h>
#include <core/Graph.h>
#include <core/Toolbox.h>

class ShellcodeSample {
public:
	ShellcodeSample();
	virtual ~ShellcodeSample();

    byte_t *code();
    ShellcodeInfo *info();
    Graph *graph();

	void setCode(byte_t *code);

private:
    byte_t *m_code;
    ShellcodeInfo *m_info;
    Graph *m_graph;
};

#endif /* SHELLCODESAMPLE_H_ */
