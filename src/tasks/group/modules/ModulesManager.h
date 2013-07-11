/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef GROUP_MODULEMANAGER_H_
#define GROUP_MODULEMANAGER_H_

#include <map>
#include <string>

namespace Group {

class IInput;
class IOutput;
class IAlgorithm;

typedef std::map<std::string, IInput *> InputMap;
typedef std::map<std::string, IOutput *> OutputMap;
typedef std::map<std::string, IAlgorithm *> AlgorithmMap;

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
    AlgorithmMap *algorithm();

private:
	ModuleManager() {}

	void loadInput();
	void loadOutput();
    void loadAlgorithm();

	void removeInput();
	void removeOutput();
    void removeAlgorithm();

	InputMap m_inputModules;
	OutputMap m_outputModules;
    AlgorithmMap m_algorithmModules;
};

} // namespace Group

#endif /* GROUP_MODULEMANAGER_H_ */
