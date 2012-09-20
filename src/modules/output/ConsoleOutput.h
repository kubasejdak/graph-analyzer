/*
 * Filename		: ConsoleOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#ifndef CONSOLEOUTPUT_H_
#define CONSOLEOUTPUT_H_

/* standard headers */
#include <iostream>
using namespace std;

#include "AbstractOutput.h"

class ConsoleOutput: public AbstractOutput {
public:
	ConsoleOutput();
	virtual ~ConsoleOutput();

	virtual bool generateOutput(ShellcodeSample *sample);
};

#endif /* CONSOLEOUTPUT_H_ */
