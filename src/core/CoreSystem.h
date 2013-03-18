/*
 *  Filename	: CoreSystem.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef CORESYSTEM_H_
#define CORESYSTEM_H_

#include <QList>
#include <QString>
#include <QDir>
#include <QFile>

#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/XMLParser.h>
#include <utils/Toolbox.h>

class CoreSystem {
public:
	CoreSystem();
	virtual ~CoreSystem() {}

	/* system interface */
	bool executeTasks();

	/* status and error */
	QString status();
	QString lastError();
	QString version();

private:
	//void loadModules();

	/* data members */
	XMLParser m_xmlParser;
};

#endif /* CORESYSTEM_H_ */
