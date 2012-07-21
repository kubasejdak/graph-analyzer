/*
 * Filename	: DecoderKey.h
 * Author	: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#ifndef DECODERKEY_H_
#define DECODERKEY_H_

/* project headers */
#include "AbstractAnalyze.h"
#include <core/Graph.h>
#include <core/InstructionSplitter.h>

class DecoderKey: public AbstractAnalyze {
public:
	DecoderKey();
	virtual ~DecoderKey();

	bool perform(ShellcodeSample *sample);
};

#endif /* DECODERKEY_H_ */
