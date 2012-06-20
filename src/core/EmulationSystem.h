/*
 *  Filename	: EmulationSystem.h
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#ifndef EMULATIONSYSTEM_H_
#define EMULATIONSYSTEM_H_

#define	EMULATION_STEPS		1000000

/* standard headers */
#include <map>

/* project headers */
#include <core/toolbox.h>
#include <core/EmulationUnit.h>
#include <core/ShellcodeSample.h>
#include <modules/AbstractAnalyze.h>
#include <modules/ModuleManager.h>

class EmulationSystem {
public:
	EmulationSystem();
	virtual ~EmulationSystem();

	void loadSample(ShellcodeSample *sample);
	bool emulate();
	bool analyze();

private:
	void loadModules();

	EmulationUnit *emuUnit;
	ShellcodeSample *sample;
	map<int, AbstractAnalyze *> *analyzeModules;
};

#endif /* EMULATIONSYSTEM_H_ */
