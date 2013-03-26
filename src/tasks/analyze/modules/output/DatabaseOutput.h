/*
 * Filename		: DatabaseOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#ifndef DATABASEOUTPUT_H_
#define DATABASEOUTPUT_H_

#include <tasks/analyze/modules/output/IOutput.h>
#include <core/ExploitInfo.h>
#include <QString>

#define ANALYZE_PREFIX		QString("analyze_")
#define SEQUENCE_POSTFIX	QString("_id_seq")

class DatabaseOutput: public IOutput {
public:
	DatabaseOutput();
    virtual ~DatabaseOutput() {}

	virtual bool exportOutput(ExploitSample *sample);

private:
    bool checkDuplicate(ExploitInfo *info);

	bool exportGeneralData(ExploitInfo *info, int id);
	bool exportAnalyzeData(ExploitInfo *info, int id);
};

#endif /* DATABASEOUTPUT_H_ */
