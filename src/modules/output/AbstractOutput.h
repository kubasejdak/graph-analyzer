/*
 *  Filename	: AbstractOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ABSTRACTOUTPUT_H_
#define ABSTRACTOUTPUT_H_

#include <modules/AbstractModule.h>
#include <core/ExploitSample.h>

class AbstractOutput: public AbstractModule {
public:
    AbstractOutput();
    virtual ~AbstractOutput() {}

    static int nextID()
	{
        return m_nextID++;
	}

	virtual bool generateOutput(ExploitSample *sample) = 0;

protected:
    static int m_nextID;
};

#endif /* ABSTRACTOUTPUT_H_ */
