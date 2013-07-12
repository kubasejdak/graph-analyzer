/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef ANALYZE_MODULEMANAGER_H_
#define ANALYZE_MODULEMANAGER_H_

#include <map>
#include <string>

namespace Analyze {

class IInput;
class IOutput;
class IAnalyze;

typedef std::map<std::string, IInput *> InputMap;
typedef std::map<std::string, IOutput *> OutputMap;
typedef std::map<std::string, IAnalyze *> AnalyzeMap;

class ModulesManager {
public:
	virtual ~ModulesManager();
	static ModulesManager *instance()
	{
		static ModulesManager m_instance;
		return &m_instance;
	}

	InputMap *input();
	OutputMap *output();
	AnalyzeMap *analyze();

private:
	ModulesManager() {}

	void loadInput();
	void loadOutput();
	void loadAnalyze();

	void removeInput();
	void removeOutput();
	void removeAnalyze();

	InputMap m_inputModules;
	OutputMap m_outputModules;
	AnalyzeMap m_analyzeModules;
};

} // namespace Analyze

#endif /* ANALYZE_MODULEMANAGER_H_ */
