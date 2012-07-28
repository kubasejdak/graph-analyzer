/*
 * Filename	: PcapInput.h
 * Author	: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef PCAPINPUT_H_
#define PCAPINPUT_H_

/* standard headers */
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

/* project headers */
#include "AbstractInput.h"
#include <toolbox.h>
#include <options.h>

class PcapInput: public AbstractInput {
public:
	PcapInput();
	virtual ~PcapInput();

	virtual void loadInput(string filename, queue<ShellcodeSample *> *samples);
};

#endif /* PCAPINPUT_H_ */
