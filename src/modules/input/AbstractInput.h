/*
 *  Filename	: AbstractInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ABSTRACTINPUT_H_
#define ABSTRACTINPUT_H_

#include <QString>
#include <QList>
#include <QFile>
#include <QQueue>

#include <modules/AbstractModule.h>
#include <core/ExploitSample.h>

class AbstractInput: public AbstractModule {
public:
    AbstractInput();
    virtual ~AbstractInput() {}

    static int nextID()
	{
        return m_nextID++;
	}

    QString type();
    virtual bool loadInput(QString filename, QList<ExploitSample *> *samples) = 0;

protected:
    static int m_nextID;
    QString m_type;
};

#endif /* ABSTRACTINPUT_H_ */
