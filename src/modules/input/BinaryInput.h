/*
 *  Filename	: BinaryInput.h
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef BINARYINPUT_H_
#define BINARYINPUT_H_

#include <QFileInfo>

#include "AbstractInput.h"
#include <core/Options.h>

class BinaryInput: public AbstractInput {
public:
	BinaryInput();
    virtual ~BinaryInput() {}

    virtual bool loadInput(QString filename, QList<ExploitSample *> *samples);
};

#endif /* BINARYINPUT_H_ */
