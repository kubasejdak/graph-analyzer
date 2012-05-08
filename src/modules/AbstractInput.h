/*
 *  Filename	: AbstractInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ABSTRACTINPUT_H_
#define ABSTRACTINPUT_H_

/* standard headers*/
#include <string>

/* project headers */
#include "AbstractModule.h"

class AbstractInput: public AbstractModule {
public:
	AbstractInput();
	virtual ~AbstractInput();

	static int getNextID() {
		return nextID++;
	}

	string getExtension();
	virtual bool loadInput(string filename) = 0;

protected:
	static int nextID;
	string extension;
};

#endif /* ABSTRACTINPUT_H_ */
