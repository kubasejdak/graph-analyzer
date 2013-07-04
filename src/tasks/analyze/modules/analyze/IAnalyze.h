/*
 *  Filename	: IAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef IANALYZE_H_
#define IANALYZE_H_

#include <string>

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>

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

#endif /* IANALYZE_H_ */
