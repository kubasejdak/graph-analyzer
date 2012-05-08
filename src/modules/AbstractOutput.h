/*
 *  Filename	: AbstractOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ABSTRACTOUTPUT_H_
#define ABSTRACTOUTPUT_H_

/* project headers */
#include "AbstractModule.h"

class AbstractOutput: public AbstractModule {
public:
	AbstractOutput();
	virtual ~AbstractOutput();

	bool generateOutput(string filename);
};

#endif /* ABSTRACTOUTPUT_H_ */
