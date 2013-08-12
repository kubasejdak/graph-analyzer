/*
 *  Filename	: SymetricSymilarity.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "SymetricSymilarity.h"

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

SymetricSimilarity::SymetricSimilarity()
{
    m_name = "SymetricSimilarity";
    m_description = "Groups samples using naive symetric similarity for given sample trait type.";
}

bool SymetricSimilarity::group(GroupTask *task, SampleList &samples, GroupManager &groupManager, AlgorithmContext &context)
{
	// extract context
    m_threshold = atoi(context.value("threshold").c_str());
    m_compareTrait = context.value("trait");
    m_compareTraitKey = context.value("traitKey");

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
            if(compare(groupManager.leader(i), sample, resemblence) == true)
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

int SymetricSimilarity::calculateSimilarity(int commonA, int commonB, int commonAB)
{
    double similarity = (double) commonAB / (double) (commonA + commonB - commonAB);
    return similarity * 100;
}

bool SymetricSimilarity::compare(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, int &similarity)
{
    TraitVector traitsA = findTraits(sampleA);
    TraitVector traitsB = findTraits(sampleB);
    TraitVector commonTraits = findCommonTraits(traitsA, traitsB);

	// check if samples have any loops
    if(traitsA.size() == 0 && traitsB.size() == 0)
		return false;

    similarity = calculateSimilarity(traitsA.size(), traitsB.size(), commonTraits.size());
    if(similarity >= m_threshold)
		return true;

	return false;
}

SymetricSimilarity::TraitVector SymetricSimilarity::findTraits(ExploitSampleHandle sample)
{
    TraitVector hashes;
    TraitMapHandle traitsMap = sample->info()->traits();

    TraitMap::iterator it = traitsMap->find(m_compareTrait);
	TraitEntry::iterator it2;
    while(it != traitsMap->end() && it.key() == m_compareTrait) {
		for(it2 = it.value()->begin(); it2 != it.value()->end(); ++it2) {
            if(it2.key() == m_compareTraitKey)
				hashes.push_back(it2.value());
		}
		++it;
    }

    return hashes;
}

SymetricSimilarity::TraitVector SymetricSimilarity::findCommonTraits(TraitVector traitsA, TraitVector traitsB)
{
    TraitVector commonTraits;

    TraitVector::iterator it;
    for(it = traitsA.begin(); it != traitsA.end(); ++it) {
        if(find(traitsB.begin(), traitsB.end(), *it) != traitsB.end())
            commonTraits.push_back(*it);
    }

    return commonTraits;
}

bool compareResemblence(pair<int, int> a, pair<int, int> b)
{
	return a.second > b.second;
}
