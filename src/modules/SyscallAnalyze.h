/*
 *  Filename	: SyscallAnalyze.h
 *  Author	: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#ifndef SYSCALLANALYZE_H_
#define SYSCALLANALYZE_H_

/* project headers */
#include "AbstractAnalyze.h"
#include <core/Graph.h>

class SyscallAnalyze: public AbstractAnalyze {
public:
	SyscallAnalyze();
	virtual ~SyscallAnalyze();

	virtual bool perform(ShellcodeSample *sample);

private:
	string cutSyscallName(string instr, string dll);
};

#endif /* SYSCALLANALYZE_H_ */
