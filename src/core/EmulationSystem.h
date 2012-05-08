/*
 *  Filename	: EmulationSystem.h
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef EMULATIONSYSTEM_H_
#define EMULATIONSYSTEM_H_

/* standard headers */
#include <list>

/* project headers */
#include <core/ShellcodeSample.h>
#include <modules/AbstractAnalyze.h>

class EmulationSystem {
public:
	EmulationSystem();
	virtual ~EmulationSystem();

	void loadSample(ShellcodeSample *sample);
	bool emulate();

private:
	ShellcodeSample *sample;
	list<AbstractAnalyze *> *analyzeModules;

	void loadModules();
};

#endif /* EMULATIONSYSTEM_H_ */
