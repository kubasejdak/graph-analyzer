/*
 *  Filename	: CoreSystem.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef CORESYSTEM_H_
#define CORESYSTEM_H_

#include <QString>

#include <utils/XMLParser.h>

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
	/* data members */
	XMLParser m_xmlParser;
};

#endif /* CORESYSTEM_H_ */
