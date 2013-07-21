/*
 *  Filename	: SymetricProbability.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "SymetricProbability.h"

#include <vector>
#include <algorithm>
#include <utility>
#include <cstdlib>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <tasks/group/GroupManager.h>
#include <tasks/group/GroupTask.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/SystemLogger.h>

using namespace std;
using namespace Group;

typedef vector<pair<int, int> > ResemblenceVector;

bool compareResemblence(pair<int, int> a, pair<int, int> b);

SymetricProbability::SymetricProbability()
{
    m_name = "SymetricProbability";
    m_description = "Groups samples using naive symetric probability.";
}

bool SymetricProbability::group(GroupTask *task, SampleList &samples, GroupManager &groupManager, AlgorithmContext &context)
{
	// extract context
	int threshold = atoi(context.value("threshold").c_str());

	// for each sample try to find appropriate group for it
	for(ExploitSampleHandle sample : samples) {
		// if no groups found (first sample), create immediately group for it
		if(groupManager.count() == 0) {
			int groupId = groupManager.createGroup();
			groupManager.add(groupId, sample, 100);

			task->incrementFoundGroups();
			task->incrementGroupedsamples();

			// export status
			task->updateStatus();
			SystemLogger::instance()->exportStatus(task);

			continue;
		}

		// for each existing group, compare its leader and current sample
		ResemblenceVector resemblenceVector;
		for(int i = 0; i < groupManager.count(); ++i) {
			int resemblence;
			if(compare(groupManager.leader(i), sample, threshold, resemblence) == true)
				resemblenceVector.push_back(make_pair(i, resemblence));
		}

		sort(resemblenceVector.begin(), resemblenceVector.end(), compareResemblence);

		if(resemblenceVector.size() > 0) {
			int group = resemblenceVector[resemblenceVector.size() - 1].first;
			int resemblence = resemblenceVector[resemblenceVector.size() - 1].second;
			LOG("adding sample [%s] to group [%d]\n", sample->info()->name().c_str(), group);
			groupManager.add(group, sample, resemblence);
		}
		else {
			int groupId = groupManager.createGroup();
			LOG("creating new group [%d] for sample [%s]\n", groupId, sample->info()->name().c_str());
			groupManager.add(groupId, sample, 100);

			task->incrementFoundGroups();
		}

		task->incrementGroupedsamples();

		// export status
		task->updateStatus();
		SystemLogger::instance()->exportStatus(task);
	}

	return true;
}

bool SymetricProbability::compare(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, int threshold, int &resemblence)
{
	HashVector loopsA = findLoopHashes(sampleA);
	HashVector loopsB = findLoopHashes(sampleB);
	HashVector commonLoops = commonLoopHashes(loopsA, loopsB);

	// check if samples have any loops
	if(loopsA.size() == 0 && loopsB.size() == 0)
		return false;

	resemblence = calculateProbability(loopsA.size(), loopsB.size(), commonLoops.size());
	if(resemblence >= threshold)
		return true;

	return false;
}

int SymetricProbability::calculateProbability(int commonA, int commonB, int commonAB)
{
	double probability = (double) commonAB / (double) (commonA + commonB - commonAB);
	return probability * 100;
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

bool compareResemblence(pair<int, int> a, pair<int, int> b)
{
	return a.second > b.second;
}
