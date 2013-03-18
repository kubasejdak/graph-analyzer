/*
 * Filename		: ConsoleOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#ifndef CONSOLEOUTPUT_H_
#define CONSOLEOUTPUT_H_

#include <iostream>
using namespace std;

#include <tasks/analyze/modules/output/IOutput.h>

class ConsoleOutput: public IOutput {
public:
	ConsoleOutput();
    virtual ~ConsoleOutput() {}

	virtual bool generateOutput(ExploitSample *sample);
};

#endif /* CONSOLEOUTPUT_H_ */
