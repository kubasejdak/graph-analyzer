/*
 *  Filename	: IAlgorithm.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef IALGORITHM_H
#define IALGORITHM_H

#include <memory>

#include <core/ExploitSample.h>
#include <tasks/group/modules/IModule.h>
#include <tasks/group/GroupManager.h>
#include <tasks/group/modules/algorithms/AlgorithmContext.h>

class GroupTask;

namespace Group {

class IAlgorithm: public IModule {
public:
	IAlgorithm() {}
	virtual ~IAlgorithm() {}

	virtual bool group(GroupTask *task, SampleList &samples, GroupManager &groupManager, AlgorithmContext &context) = 0;
};

typedef std::shared_ptr<IAlgorithm> IAlgorithmHandle;

} // namespace Group

#endif /* IALGORITHM_H */
