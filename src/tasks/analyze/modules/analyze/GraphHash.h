/*
 * Filename		: GraphHash.h
 * Author		: Kuba Sejdak
 * Created on	: 23-07-2012
 */

#ifndef GRAPHHASH_H_
#define GRAPHHASH_H_

#include <tasks/analyze/modules/analyze/IAnalyze.h>

class ExploitSample;

class GraphHash: public IAnalyze {
public:
	GraphHash();
    virtual ~GraphHash() {}

	virtual bool perform(ExploitSampleHandle sample);
	virtual bool exportToDatabase(ExploitSampleHandle sample, int sampleId);
	virtual bool importFromDatabase(ExploitSampleHandle sample, int sampleId);
};

#endif /* GRAPHHASH_H_ */
