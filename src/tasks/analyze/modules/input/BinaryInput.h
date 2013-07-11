/*
 *  Filename	: BinaryInput.h
 *  Author	: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef ANALYZE_BINARYINPUT_H_
#define ANALYZE_BINARYINPUT_H_

#include <string>

#include <tasks/analyze/modules/input/IInput.h>
#include <core/ExploitSample.h>

namespace Analyze {

class BinaryInput: public IInput {
public:
	BinaryInput();
    virtual ~BinaryInput() {}

	virtual bool loadInput(std::string filename, SampleList *samples);
};

} // namespace Analyze

#endif /* ANALYZE_BINARYINPUT_H_ */
