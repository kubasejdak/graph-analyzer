/*
 * Filename		: GraphHash.h
 * Author		: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#ifndef GRAPHHASH_H_
#define GRAPHHASH_H_

#include "AbstractAnalyze.h"
#include <core/Toolbox.h>
#include <core/Graph.h>
#include <core/InstructionSplitter.h>

class GraphHash: public AbstractAnalyze {
public:
	GraphHash();
    virtual ~GraphHash() {}

	virtual bool perform(ExploitSample *sample);
	virtual bool exportToDatabase(ExploitSample *sample, int sampleId);
};

#endif /* GRAPHHASH_H_ */
