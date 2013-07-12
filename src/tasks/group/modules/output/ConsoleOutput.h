/*
 * Filename		: ConsoleOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2013
 */

#ifndef GROUP_CONSOLEOUTPUT_H_
#define GROUP_CONSOLEOUTPUT_H_

#include <tasks/group/modules/output/IOutput.h>
#include <core/ExploitGroup.h>

namespace Group {

class ConsoleOutput: public IOutput {
public:
	ConsoleOutput();
    virtual ~ConsoleOutput() {}

	virtual bool exportOutput(ExploitGroupHandle sample, int taskId, bool duplicate);
};

} // namespace Group

#endif /* GROUP_CONSOLEOUTPUT_H_ */
