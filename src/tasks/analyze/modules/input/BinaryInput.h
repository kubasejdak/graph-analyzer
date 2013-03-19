/*
 *  Filename	: BinaryInput.h
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef BINARYINPUT_H_
#define BINARYINPUT_H_

#include <tasks/analyze/modules/input/IInput.h>

class BinaryInput: public IInput {
public:
	BinaryInput();
    virtual ~BinaryInput() {}

	virtual bool loadInput(QString filename, SampleContainer *samples);
};

#endif /* BINARYINPUT_H_ */
