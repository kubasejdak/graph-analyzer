/*
 *  Filename	: SymetricProbability.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "SymetricProbability.h"

#include <tasks/group/algorithms/IAlgorithm.h>
#include <core/ExploitSample.h>

bool SymetricProbability::process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context)
{
	return true;
}

int SymetricProbability::calculateProbability(int commonA, int commonB, int commonAB)
{
	return (commonAB / (commonA + commonB - commonAB)) * 100;
}
