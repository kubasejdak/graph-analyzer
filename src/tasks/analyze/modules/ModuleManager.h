/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <map>
#include <string>

class IInput;
class IOutput;
class IAnalyze;

typedef std::map<std::string, IInput *> InputMap;
typedef std::map<std::string, IOutput *> OutputMap;
typedef std::map<std::string, IAnalyze *> AnalyzeMap;

class ModuleManager {
public:
	virtual ~ModuleManager();
    static ModuleManager *instance()
	{
		static ModuleManager m_instance;
		return &m_instance;
	}

	InputMap *input();
	OutputMap *output();
	AnalyzeMap *analyze();

private:
	ModuleManager() {}

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

#endif /* MODULEMANAGER_H_ */
