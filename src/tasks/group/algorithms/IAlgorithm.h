/*
 *  Filename	: IAlgorithm.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef IALGORITHM_H
#define IALGORITHM_H

#include <memory>

#include <core/ExploitSample.h>
#include <tasks/group/algorithms/AlgorithmContext.h>

class IAlgorithm {
public:
	IAlgorithm() {}
	virtual ~IAlgorithm() {}

	virtual bool process(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, AlgorithmContext &context) = 0;
};

typedef std::shared_ptr<IAlgorithm> IAlgorithmHandle;

#endif /* IALGORITHM_H */
