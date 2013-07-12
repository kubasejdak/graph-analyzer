/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#ifndef ANALYZE_DATABASEOUTPUT_H_
#define ANALYZE_DATABASEOUTPUT_H_

#include <tasks/analyze/modules/output/IOutput.h>

class ExploitSample;
class ExploitInfo;

namespace Analyze {

class DatabaseOutput: public IOutput {
public:
	DatabaseOutput();
    virtual ~DatabaseOutput() {}

	virtual bool exportOutput(ExploitSampleHandle sample, int taskId, bool duplicate);

private:
    bool checkDuplicate(ExploitInfoHandle info);

    bool exportGeneralData(ExploitInfoHandle info, int sampleId, int taskId);
};

} // namespace Analyze

#endif /* ANALYZE_DATABASEOUTPUT_H_ */
