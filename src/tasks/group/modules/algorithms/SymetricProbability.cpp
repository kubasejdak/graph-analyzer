/*
 *  Filename	: SymetricProbability.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "SymetricProbability.h"

#include <vector>
#include <algorithm>
#include <cstdlib>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>

using namespace std;
using namespace Group;

SymetricProbability::SymetricProbability()
{
    m_name = "SymetricProbability";
    m_description = "Groups samples using naive symetric probability.";
}

bool SymetricProbability::process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context)
{
	HashVector loopsA = findLoopHashes(sampleA);
	HashVector loopsB = findLoopHashes(sampleB);
	HashVector commonLoops = commonLoopHashes(loopsA, loopsB);

	// check if samples have any loops
	if(loopsA.size() == 0 && loopsB.size() == 0)
		return false;

	int treshold = atoi(context.value("threshold").c_str());

	if(calculateProbability(loopsA.size(), loopsB.size(), commonLoops.size()) >= treshold)
		return true;

	return false;
}

int SymetricProbability::calculateProbability(int commonA, int commonB, int commonAB)
{
	return (commonAB / (commonA + commonB - commonAB)) * 100;
}

SymetricProbability::HashVector SymetricProbability::findLoopHashes(ExploitSampleHandle sample)
{
    HashVector hashes;
    TraitMapHandle traits = sample->info()->traits();

    TraitMap::iterator it = traits->find("loop");
	TraitEntry::iterator it2;
    while(it != traits->end() && it.key() == "loop") {
		for(it2 = it.value()->begin(); it2 != it.value()->end(); ++it2) {
			if(it2.key() == "hash")
				hashes.push_back(it2.value());
		}
		++it;
    }

    return hashes;
}

SymetricProbability::HashVector SymetricProbability::commonLoopHashes(HashVector loopsA, HashVector loopsB)
{
    HashVector commonLoops;

    HashVector::iterator it;;
    for(it = loopsA.begin(); it != loopsA.end(); ++it) {
        if(find(loopsB.begin(), loopsB.end(), *it) != loopsB.end())
            commonLoops.push_back(*it);
    }

    return commonLoops;
}
