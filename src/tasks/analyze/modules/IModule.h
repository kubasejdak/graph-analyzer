/*
 *  Filename	: IModule.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef ANALYZE_IMODULE_H_
#define ANALYZE_IMODULE_H_

#include <string>

namespace Analyze {

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

#endif /* ANALYZE_IMODULE_H_ */
