/*
 *  Filename	: BinaryInput.h
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef BINARYINPUT_H_
#define BINARYINPUT_H_

/* project headers */
#include "AbstractInput.h"
#include <core/toolbox.h>

class BinaryInput: public AbstractInput {
public:
	BinaryInput();
	virtual ~BinaryInput();

	virtual bool loadInput(string filename, ShellcodeSample *sample);
};

#endif /* BINARYINPUT_H_ */
