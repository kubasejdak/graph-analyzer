/*
 *  Filename	: AbstractAnalyze.h
 *  Author	: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef ABSTRACTANALYZE_H_
#define ABSTRACTANALYZE_H_

/* project headers */
#include <modules/AbstractModule.h>
#include <core/ShellcodeSample.h>

class AbstractAnalyze : public AbstractModule {
public:
	AbstractAnalyze();
	virtual ~AbstractAnalyze();

	static int getNextID() {
		return nextID++;
	}

	virtual bool perform(ShellcodeSample *sample) = 0;

protected:
	static int nextID;
};

#endif /* ABSTRACTANALYZE_H_ */
