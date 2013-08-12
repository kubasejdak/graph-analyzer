/*
 *  Filename	: SymetricSymilarity.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef GROUP_SYMETRICSYMILARITY_H
#define GROUP_SYMETRICSYMILARITY_H

#include <vector>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <tasks/group/GroupManager.h>
#include <core/ExploitSample.h>

namespace Group {

class SymetricSimilarity: public IAlgorithm {
public:
    SymetricSimilarity();
    virtual ~SymetricSimilarity() {}

	virtual bool group(GroupTask *task, SampleList &samples, GroupManager &groupManager, AlgorithmContext &context);

private:
    typedef std::vector<std::string> TraitVector;

    int m_threshold;
    std::string m_compareTrait;
    std::string m_compareTraitKey;

    bool compare(ExploitSampleHandle sampleA, ExploitSampleHandle sampleB, int &similarity);
    int calculateSimilarity(int commonA, int commonB, int commonAB);

    TraitVector findTraits(ExploitSampleHandle sample);
    TraitVector findCommonTraits(TraitVector sampleA, TraitVector sampleB);

};

} // namespace Group

#endif /* GROUP_SYMETRICSYMILARITY_H */
