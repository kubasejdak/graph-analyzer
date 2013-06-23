/*
 *  Filename	: IOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef IOUTPUT_H_
#define IOUTPUT_H_

#include <tasks/analyze/modules/IModule.h>

class ExploitSample;

class IOutput: public IModule {
public:
	IOutput() {}
	virtual ~IOutput() {}

    virtual bool exportOutput(ExploitSample *sample, int taskId) = 0;
};

#endif /* IOUTPUT_H_ */
