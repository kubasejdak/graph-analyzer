/*
 * Filename		: PcapInput.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef PCAPINPUT_H_
#define PCAPINPUT_H_

#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "AbstractInput.h"
#include <core/SystemLogger.h>
#include <core/Options.h>
#include <core/Toolbox.h>

class PcapInput: public AbstractInput {
public:
	PcapInput();
    virtual ~PcapInput() {}

    virtual bool loadInput(QString filename, QList<ExploitSample *> *samples);
};

#endif /* PCAPINPUT_H_ */
