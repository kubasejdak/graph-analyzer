/*
 *  Filename	: SyscallAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#ifndef SYSCALLANALYZE_H_
#define SYSCALLANALYZE_H_

#include "AbstractAnalyze.h"
#include <core/Graph.h>
#include <utils/InstructionSplitter.h>

class SyscallAnalyze: public AbstractAnalyze {
public:
	SyscallAnalyze();
    virtual ~SyscallAnalyze() {}

	virtual bool perform(ExploitSample *sample);
	virtual bool exportToDatabase(ExploitSample *sample, int sampleId);
};

#endif /* SYSCALLANALYZE_H_ */
