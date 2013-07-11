/*
 *  Filename	: SymetricProbability.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef GROUP_SYMETRICPROBABILITY_H
#define GROUP_SYMETRICPROBABILITY_H

#include <vector>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <core/ExploitSample.h>

namespace Group {

class SymetricProbability: public IAlgorithm {
public:
    SymetricProbability();
	virtual ~SymetricProbability() {}

	virtual bool process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context);

private:
	typedef std::vector<std::string> HashVector;

	int calculateProbability(int commonA, int commonB, int commonAB);

	HashVector findLoopHashes(ExploitSampleHandle sample);
	HashVector commonLoopHashes(HashVector loopsA, HashVector loopsB);

};

} // namespace Group

#endif /* GROUP_SYMETRICPROBABILITY_H */
