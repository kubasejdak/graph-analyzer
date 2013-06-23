/*
 * Filename		: PcapInput.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef PCAPINPUT_H_
#define PCAPINPUT_H_

#include <string>

#include <tasks/analyze/modules/input/IInput.h>
#include <core/ExploitSample.h>

class PcapInput: public IInput {
public:
	PcapInput();
    virtual ~PcapInput() {}

	virtual bool loadInput(std::string filename, SampleContainer *samples);
};

#endif /* PCAPINPUT_H_ */
