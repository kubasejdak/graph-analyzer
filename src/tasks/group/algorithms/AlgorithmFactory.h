/*
 *  Filename	: AlgorithmFactory.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef ALGORITHMFACTORY_H
#define ALGORITHMFACTORY_H

#include <string>

#include <tasks/group/algorithms/IAlgorithm.h>

class AlgorithmFactory {
public:
	AlgorithmFactory() {}
	virtual ~AlgorithmFactory() {}

	IAlgorithmHandle createAlgorithm(std::string name);
};

#endif /* ALGORITHMFACTORY_H */
