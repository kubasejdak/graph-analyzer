/*
 *  Filename	: IModule.h
 *  Author		: Kuba Sejdak
 *  Created on	: 21-07-2013
 */

#ifndef EXPORT_IMODULE_H_
#define EXPORT_IMODULE_H_

#include <string>

namespace Export {

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

} // namespace Export

#endif /* EXPORT_IMODULE_H_ */
