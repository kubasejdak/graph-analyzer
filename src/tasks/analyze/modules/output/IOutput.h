/*
 *  Filename	: IOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ANALYZE_IOUTPUT_H_
#define ANALYZE_IOUTPUT_H_

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>

namespace Analyze {

class IOutput: public IModule {
public:
	IOutput() {}
	virtual ~IOutput() {}

	virtual bool exportOutput(ExploitSampleHandle sample, int taskId, bool duplicate) = 0;
};

} // namespace Analyze

#endif /* ANALYZE_IOUTPUT_H_ */
