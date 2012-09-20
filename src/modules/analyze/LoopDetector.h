/*
 * Filename		: LoopDetector.h
 * Author		: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#ifndef LOOPDETECTOR_H_
#define LOOPDETECTOR_H_

/* standard headers */
#include <sstream>
#include <map>

/* project headers */
#include "AbstractAnalyze.h"
#include <core/Graph.h>
#include <toolbox.h>

class LoopDetector: public AbstractAnalyze {
public:
	LoopDetector();
	virtual ~LoopDetector();

	virtual bool perform(ShellcodeSample *sample);
};

#endif /* LOOPDETECTOR_H_ */
