/*
 * Filename		: GraphHash.h
 * Author		: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#ifndef GRAPHHASH_H_
#define GRAPHHASH_H_

#include <tasks/analyze/modules/analyze/IAnalyze.h>

class GraphHash: public IAnalyze {
public:
	GraphHash();
    virtual ~GraphHash() {}

	virtual bool perform(ExploitSample *sample);
	virtual bool exportToDatabase(ExploitSample *sample, int sampleId);
};

#endif /* GRAPHHASH_H_ */
