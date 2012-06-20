/*
 *  Filename	: AbstractAnalyze.h
 *  Author	: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef ABSTRACTANALYZE_H_
#define ABSTRACTANALYZE_H_

/* libemu headers */
extern "C" {
	#include <emu/emu.h>
	#include <emu/emu_cpu.h>
	#include <emu/emu_memory.h>
}

/* project headers */
#include "modules/AbstractModule.h"
#include <core/EmulationUnit.h>

class AbstractAnalyze : public AbstractModule {
public:
	AbstractAnalyze();
	virtual ~AbstractAnalyze();

	virtual bool perform(EmulationUnit *emuUnit) = 0;
};

#endif /* ABSTRACTANALYZE_H_ */
