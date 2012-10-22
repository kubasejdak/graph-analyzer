/*
 * Filename		: LoopDetector.h
 * Author		: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#ifndef LOOPDETECTOR_H_
#define LOOPDETECTOR_H_

#include "AbstractAnalyze.h"
#include <core/Graph.h>
#include <core/Toolbox.h>

class LoopDetector: public AbstractAnalyze {
public:
	LoopDetector();
    virtual ~LoopDetector() {}

	virtual bool perform(ShellcodeSample *sample);
};

#endif /* LOOPDETECTOR_H_ */
