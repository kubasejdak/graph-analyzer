/*
 *  Filename	: IInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 12-07-2013
 */

#ifndef GROUP_IINPUT_H_
#define GROUP_IINPUT_H_

#include <string>

#include <tasks/group/modules/IModule.h>
#include <core/ExploitSample.h>

class GroupTask;

namespace Group {

class IInput: public IModule {
public:
	IInput() {}
	virtual ~IInput() {}

	std::string type() { return m_type; }
	virtual bool loadInput(GroupTask *context, SampleList *samples) = 0;

protected:
	std::string m_type;
};

} // namespace Group

#endif /* GROUP_IINPUT_H_ */
