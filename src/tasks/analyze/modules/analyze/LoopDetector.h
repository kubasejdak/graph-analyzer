/*
 * Filename		: LoopDetector.h
 * Author		: Kuba Sejdak
 * Created on	: 13-07-2012
 */

#ifndef ANALYZE_LOOPDETECTOR_H_
#define ANALYZE_LOOPDETECTOR_H_

#include <tasks/analyze/modules/analyze/IAnalyze.h>

class ExploitSample;

namespace Analyze {

class LoopDetector: public IAnalyze {
public:
	LoopDetector();
    virtual ~LoopDetector() {}

	virtual bool perform(ExploitSampleHandle sample);
	virtual bool exportToDatabase(ExploitSampleHandle sample, int sampleId);
	virtual bool importFromDatabase(ExploitSampleHandle sample, int sampleId);
};

} // namespace Analyze

#endif /* ANALYZE_LOOPDETECTOR_H_ */
