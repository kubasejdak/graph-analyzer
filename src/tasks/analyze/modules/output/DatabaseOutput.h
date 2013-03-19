/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#ifndef DATABASEOUTPUT_H_
#define DATABASEOUTPUT_H_

#include <tasks/analyze/modules/output/IOutput.h>
#include <core/ExploitInfo.h>

class DatabaseOutput: public IOutput {
public:
	DatabaseOutput();
    virtual ~DatabaseOutput() {}

	virtual bool exportOutput(ExploitSample *sample);

private:
    bool checkDuplicate(ExploitInfo *info);
};

#endif /* DATABASEOUTPUT_H_ */
