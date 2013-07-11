/*
 *  Filename	: SyscallAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 13-05-2012
 */

#ifndef ANALYZE_SYSCALLANALYZE_H_
#define ANALYZE_SYSCALLANALYZE_H_

#include <tasks/analyze/modules/analyze/IAnalyze.h>

class ExploitSample;

namespace Analyze {

class SyscallAnalyze: public IAnalyze {
public:
	SyscallAnalyze();
    virtual ~SyscallAnalyze() {}

	virtual bool perform(ExploitSampleHandle sample);
	virtual bool exportToDatabase(ExploitSampleHandle sample, int sampleId);
	virtual bool importFromDatabase(ExploitSampleHandle sample, int sampleId);
};

} // namespace Analyze

#endif /* ANALYZE_SYSCALLANALYZE_H_ */
