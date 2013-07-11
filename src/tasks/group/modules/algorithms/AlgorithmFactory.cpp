/*
 *  Filename	: AlgorithmFactory.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "AlgorithmFactory.h"

#include <string>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <tasks/group/modules/algorithms/SymetricProbability.h>

using namespace std;
using namespace Group;

IAlgorithmHandle AlgorithmFactory::createAlgorithm(string name)
{
	if(name == "symetric")
		return IAlgorithmHandle(new SymetricProbability());

	return NULL;
}
