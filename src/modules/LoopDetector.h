/*
 * Filename	: LoopDetector.h
 * Author	: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#ifndef LOOPDETECTOR_H_
#define LOOPDETECTOR_H_

/* standard headers */
#include <sstream>

/* project headers */
#include "AbstractAnalyze.h"
#include <core/Graph.h>

class LoopDetector: public AbstractAnalyze {
public:
	LoopDetector();
	virtual ~LoopDetector();

	bool perform(ShellcodeSample *sample);

private:
	string mergeLoopDesc(int address, int iter);
};

#endif /* LOOPDETECTOR_H_ */
