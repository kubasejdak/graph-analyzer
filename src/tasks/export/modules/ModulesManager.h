/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 21-07-2013
 */

#ifndef EXPORT_MODULEMANAGER_H_
#define EXPORT_MODULEMANAGER_H_

#include <map>
#include <string>

namespace Export {

class IOutput;

typedef std::map<std::string, IOutput *> OutputMap;

class ModulesManager {
public:
	virtual ~ModulesManager();
	static ModulesManager *instance()
	{
		static ModulesManager m_instance;
		return &m_instance;
	}

	OutputMap *output();

private:
	ModulesManager() {}

	void loadOutput();

	void removeOutput();

	OutputMap m_outputModules;
};

} // namespace Export

#endif /* EXPORT_MODULEMANAGER_H_ */
