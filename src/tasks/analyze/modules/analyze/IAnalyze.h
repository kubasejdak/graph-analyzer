/*
 *  Filename	: IAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef IANALYZE_H_
#define IANALYZE_H_

#include <QMap>
#include <QtSql>

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/DatabaseManager.h>

class IAnalyze: public IModule {
public:
	IAnalyze() {}
	virtual ~IAnalyze() {}

	virtual bool perform(ExploitSample *sample) = 0;
	virtual bool exportToDatabase(ExploitSample *sample, int sampleId) = 0;

protected:
    QString m_traitName;
};

#endif /* IANALYZE_H_ */
