/*
 *  Filename	: AlgorithmFactory.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "AlgorithmFactory.h"

#include <string>

#include <tasks/group/algorithms/IAlgorithm.h>
#include <tasks/group/algorithms/SymetricProbability.h>

using namespace std;

IAlgorithmHandle AlgorithmFactory::createAlgorithm(string name)
{
	if(name == "symetric")
		return IAlgorithmHandle(new SymetricProbability());

	return NULL;
}
