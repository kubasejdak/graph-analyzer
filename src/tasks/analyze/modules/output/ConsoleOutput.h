/*
 * Filename		: ConsoleOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#ifndef ANALYZE_CONSOLEOUTPUT_H_
#define ANALYZE_CONSOLEOUTPUT_H_

#include <tasks/analyze/modules/output/IOutput.h>

class ExploitSample;

namespace Analyze {

class ConsoleOutput: public IOutput {
public:
	ConsoleOutput();
    virtual ~ConsoleOutput() {}

	virtual bool exportOutput(ExploitSampleHandle sample, int taskId, bool override);
};

} // namespace Analyze

#endif /* ANALYZE_CONSOLEOUTPUT_H_ */
