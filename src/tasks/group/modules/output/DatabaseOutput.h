/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2013
 */

#ifndef GROUP_DATABASEOUTPUT_H
#define GROUP_DATABASEOUTPUT_H

#include <tasks/group/modules/output/IOutput.h>
#include <core/ExploitGroup.h>

namespace Group {

class DatabaseOutput: public IOutput {
public:
	DatabaseOutput();
	virtual ~DatabaseOutput() {}

	virtual bool exportOutput(ExploitGroupHandle group, int taskId, bool duplicate);

private:
	bool exportGeneralData(ExploitGroupHandle group, int groupId, int taskId);
	bool exportMembersData(ExploitGroupHandle group, int groupId);
};

} // namespace Group

#endif /* GROUP_DATABASEOUTPUT_H */
