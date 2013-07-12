/*
 *  Filename	: DatabaseInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 12-07-2013
 */

#ifndef GROUP_DATABASEINPUT_H_
#define GROUP_DATABASEINPUT_H_

#include <tasks/group/modules/input/IInput.h>
#include <core/ExploitSample.h>

class GroupTask;

namespace Group {

class DatabaseInput: public IInput {
public:
	DatabaseInput();
	virtual ~DatabaseInput() {}

	virtual bool loadInput(GroupTask *context, SampleList *samples);
};

} // namespace Group

#endif /* GROUP_DATABASEINPUT_H_ */
