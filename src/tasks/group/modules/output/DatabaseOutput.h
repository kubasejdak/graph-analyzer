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

	virtual bool exportOutput(ExploitGroupHandle sample, int taskId, bool duplicate);
};

} // namespace Group

#endif /* GROUP_DATABASEOUTPUT_H */
