/*
 *  Filename	: IModule.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef IMODULE_H_
#define IMODULE_H_

#include <string>

class IModule {
public:
	IModule() {}
	virtual ~IModule() {}

	std::string name() { return m_name; }
	std::string description() { return m_description; }

protected:
	std::string m_name;
	std::string m_description;
};

#endif /* IMODULE_H_ */
