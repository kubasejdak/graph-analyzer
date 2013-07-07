/*
 *  Filename	: SymetricProbability.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef SYMETRICPROBABILITY_H
#define SYMETRICPROBABILITY_H

#include <tasks/group/algorithms/IAlgorithm.h>
#include <core/ExploitSample.h>

class SymetricProbability: public IAlgorithm {
public:
	SymetricProbability() {}
	virtual ~SymetricProbability() {}

	virtual bool process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context);

private:
	int calculateProbability(int commonA, int commonB, int commonAB);
};

#endif /* SYMETRICPROBABILITY_H */
