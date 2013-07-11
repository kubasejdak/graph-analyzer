/*
 *  Filename	: IAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef ANALYZE_IANALYZE_H_
#define ANALYZE_IANALYZE_H_

#include <string>

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>

namespace Analyze {

class IAnalyze: public IModule {
public:
	IAnalyze() {}
	virtual ~IAnalyze() {}

	virtual bool perform(ExploitSampleHandle sample) = 0;
	virtual bool exportToDatabase(ExploitSampleHandle sample, int sampleId) = 0;
	virtual bool importFromDatabase(ExploitSampleHandle sample, int sampleId) = 0;

protected:
	std::string m_traitName;
};

} // namespace Analyze

#endif /* ANALYZE_IANALYZE_H_ */
