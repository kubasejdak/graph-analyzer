/*
 *  Filename	: AlgorithmFactory.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef GROUP_ALGORITHMFACTORY_H
#define GROUP_ALGORITHMFACTORY_H

#include <string>

#include <tasks/group/modules/algorithms/IAlgorithm.h>

namespace Group {

class AlgorithmFactory {
public:
	AlgorithmFactory() {}
	virtual ~AlgorithmFactory() {}

	IAlgorithmHandle createAlgorithm(std::string name);
};

} // namespace Group

#endif /* GROUP_ALGORITHMFACTORY_H */
