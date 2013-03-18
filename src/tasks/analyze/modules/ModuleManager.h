/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <QMap>
#include <QList>

#include <tasks/analyze/modules/input/IInput.h>
#include <tasks/analyze/modules/output/IOutput.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>
#include <tasks/analyze/modules/input/BinaryInput.h>
#include <tasks/analyze/modules/input/PcapInput.h>
#include <tasks/analyze/modules/output/DatabaseOutput.h>
#include <tasks/analyze/modules/output/ConsoleOutput.h>
#include <tasks/analyze/modules/analyze/SyscallAnalyze.h>
#include <tasks/analyze/modules/analyze/LoopDetector.h>
#include <tasks/analyze/modules/analyze/GraphHash.h>

typedef QMap<QString, IInput *> InputMap;
typedef QMap<QString, IOutput *> OutputMap;
typedef QMap<QString, IAnalyze *> AnalyzeMap;

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
