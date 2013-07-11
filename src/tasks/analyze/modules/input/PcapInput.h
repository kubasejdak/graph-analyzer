/*
 * Filename		: PcapInput.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef ANALYZE_PCAPINPUT_H_
#define ANALYZE_PCAPINPUT_H_

#include <string>

#include <tasks/analyze/modules/input/IInput.h>
#include <core/ExploitSample.h>

namespace Analyze {

class PcapInput: public IInput {
public:
	PcapInput();
    virtual ~PcapInput() {}

	virtual bool loadInput(std::string filename, SampleList *samples);
};

} // namespace Analyze

#endif /* ANALYZE_PCAPINPUT_H_ */
