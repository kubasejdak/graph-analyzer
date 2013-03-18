/*
 *  Filename	: IModule.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef IMODULE_H_
#define IMODULE_H_

#include <QString>

class IModule {
public:
	IModule() {}
	virtual ~IModule() {}

	QString name() { return m_name; }
	QString description() { return m_description; }

protected:
    QString m_name;
    QString m_description;
};

#endif /* IMODULE_H_ */
