/*
 *  Filename	: SymetricProbability.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef SYMETRICPROBABILITY_H
#define SYMETRICPROBABILITY_H

#include <vector>

#include <tasks/group/algorithms/IAlgorithm.h>
#include <core/ExploitSample.h>

class SymetricProbability: public IAlgorithm {
public:
	SymetricProbability() {}
	virtual ~SymetricProbability() {}

	virtual bool process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context);

private:
	typedef std::vector<std::string> HashVector;

	int calculateProbability(int commonA, int commonB, int commonAB);

	HashVector findLoopHashes(ExploitSampleHandle sample);
	HashVector commonLoopHashes(HashVector loopsA, HashVector loopsB);

};

#endif /* SYMETRICPROBABILITY_H */
