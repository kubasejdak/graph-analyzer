/*
 *  Filename	: AbstractAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef ABSTRACTANALYZE_H_
#define ABSTRACTANALYZE_H_

#include <QMap>

#include <modules/AbstractModule.h>
#include <core/ExploitSample.h>

class AbstractAnalyze: public AbstractModule {
public:
    AbstractAnalyze();
    virtual ~AbstractAnalyze() {}

    static int nextID()
	{
        return m_nextID++;
	}

	virtual bool perform(ExploitSample *sample) = 0;

protected:
    static int m_nextID;
    QString m_traitName;
};

#endif /* ABSTRACTANALYZE_H_ */
