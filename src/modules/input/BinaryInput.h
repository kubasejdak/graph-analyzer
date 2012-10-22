/*
 *  Filename	: BinaryInput.h
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef BINARYINPUT_H_
#define BINARYINPUT_H_

#include "AbstractInput.h"

class BinaryInput: public AbstractInput {
public:
	BinaryInput();
    virtual ~BinaryInput() {}

    virtual void loadInput(QString filename, QList<ShellcodeSample *> *samples);
};

#endif /* BINARYINPUT_H_ */
