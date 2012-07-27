/*
 * Filename	: GraphHash.h
 * Author	: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#ifndef GRAPHHASH_H_
#define GRAPHHASH_H_

/* project headers */
#include "AbstractAnalyze.h"
#include <toolbox.h>
#include <core/Graph.h>
#include <core/InstructionSplitter.h>

class GraphHash: public AbstractAnalyze {
public:
	GraphHash();
	virtual ~GraphHash();

	virtual bool perform(ShellcodeSample *sample);
};

#endif /* GRAPHHASH_H_ */
