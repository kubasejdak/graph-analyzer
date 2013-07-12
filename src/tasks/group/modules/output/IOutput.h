/*
 *  Filename	: IOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 12-07-2013
 */

#ifndef GROUP_IOUTPUT_H_
#define GROUP_IOUTPUT_H_

#include <tasks/group/modules/IModule.h>
#include <core/ExploitGroup.h>

namespace Group {

class IOutput: public IModule {
public:
	IOutput() {}
	virtual ~IOutput() {}

	virtual bool exportOutput(ExploitGroupHandle group, int taskId, bool override) = 0;
};

} // namespace Group

#endif /* GROUP_IOUTPUT_H_ */
