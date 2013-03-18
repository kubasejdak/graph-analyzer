/*
 *  Filename	: IInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef IINPUT_H_
#define IINPUT_H_

#include <QString>
#include <QList>
#include <QFile>
#include <QQueue>

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>

class IInput: public IModule {
public:
	IInput();
	virtual ~IInput() {}

	QString type() { return m_type; }
	virtual bool loadInput(QString filename, SampleContainer *samples) = 0;

protected:
    QString m_type;
};

#endif /* IINPUT_H_ */
