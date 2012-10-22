/*
 *  Filename	: AbstractOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ABSTRACTOUTPUT_H_
#define ABSTRACTOUTPUT_H_

#include <modules/AbstractModule.h>
#include <core/ShellcodeSample.h>

class AbstractOutput: public AbstractModule {
public:
    AbstractOutput();
    virtual ~AbstractOutput() {}

    static int nextID()
	{
        return m_nextID++;
	}

	virtual bool generateOutput(ShellcodeSample *sample) = 0;

protected:
    static int m_nextID;
};

#endif /* ABSTRACTOUTPUT_H_ */
