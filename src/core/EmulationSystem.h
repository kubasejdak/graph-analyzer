/*
 *  Filename	: EmulationSystem.h
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef EMULATIONSYSTEM_H_
#define EMULATIONSYSTEM_H_

#define	EMULATION_STEPS	1000000

/* project headers */
#include <core/toolbox.h>
#include <core/EmulationUnit.h>
#include <core/ShellcodeSample.h>

class EmulationSystem {
public:
	EmulationSystem();
	virtual ~EmulationSystem();

	void loadSample(ShellcodeSample *sample);
	bool emulate();

private:
	EmulationUnit *emuUnit;
	ShellcodeSample *sample;
};

#endif /* EMULATIONSYSTEM_H_ */
