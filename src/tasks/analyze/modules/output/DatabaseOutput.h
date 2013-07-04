/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#ifndef DATABASEOUTPUT_H_
#define DATABASEOUTPUT_H_

#include <tasks/analyze/modules/output/IOutput.h>

class ExploitSample;
class ExploitInfo;

class DatabaseOutput: public IOutput {
public:
	DatabaseOutput();
    virtual ~DatabaseOutput() {}

	virtual bool exportOutput(ExploitSampleHandle sample, int taskId, bool override);

private:
    bool checkDuplicate(ExploitInfoHandle info);

    bool exportGeneralData(ExploitInfoHandle info, int sampleId, int taskId);
};

#endif /* DATABASEOUTPUT_H_ */
