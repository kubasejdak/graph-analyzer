/*
 *  Filename	: SymetricProbability.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "SymetricProbability.h"

#include <vector>
#include <cstdlib>

#include <tasks/group/algorithms/IAlgorithm.h>
#include <core/ExploitSample.h>

using namespace std;

bool SymetricProbability::process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context)
{
	HashVector loopsA = findLoopHashes(sampleA);
	HashVector loopsB = findLoopHashes(sampleB);
	HashVector commonLoops = commonLoopHashes(loopsA, loopsB);

	int treshold = atoi(context.value("treshold").c_str());

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
}

SymetricProbability::HashVector SymetricProbability::commonLoopHashes(HashVector loopsA, HashVector loopsB)
{

}
