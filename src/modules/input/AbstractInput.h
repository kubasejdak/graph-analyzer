/*
 *  Filename	: AbstractInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ABSTRACTINPUT_H_
#define ABSTRACTINPUT_H_

/* standard headers*/
#include <string>
#include <fstream>
#include <queue>
using namespace std;

/* project headers */
#include <modules/AbstractModule.h>
#include <core/ShellcodeSample.h>

class AbstractInput: public AbstractModule {
public:
	AbstractInput();
	virtual ~AbstractInput();

	static int getNextID()
	{
		return nextID++;
	}

	string getType();
	virtual void loadInput(string filename, list<ShellcodeSample *> *samples) = 0;

protected:
	static int nextID;
	string type;
};

#endif /* ABSTRACTINPUT_H_ */
