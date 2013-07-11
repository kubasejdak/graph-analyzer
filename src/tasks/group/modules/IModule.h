/*
 *  Filename	: IModule.h
 *  Author		: Kuba Sejdak
 *  Created on	: 11-07-2013
 */

#ifndef GROUP_IMODULE_H_
#define GROUP_IMODULE_H_

#include <string>

namespace Group {

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

} // namespace Analyze

#endif /* GROUP_IMODULE_H_ */
