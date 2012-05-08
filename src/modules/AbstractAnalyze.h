/*
 *  Filename	: AbstractAnalyze.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#ifndef ABSTRACTANALYZE_H_
#define ABSTRACTANALYZE_H_

/* project headers */
#include "modules/AbstractModule.h"
#include <core/toolbox.h>

class AbstractAnalyze : public AbstractModule {
public:
	AbstractAnalyze();
	virtual ~AbstractAnalyze();

	virtual bool perform(byte_t *code) = 0;
};

#endif /* ABSTRACTANALYZE_H_ */
