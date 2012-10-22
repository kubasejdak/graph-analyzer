/*
 *  Filename	: ModuleManager.h
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <QMap>
#include <QList>

#include <modules/input/AbstractInput.h>
#include <modules/output/AbstractOutput.h>
#include <modules/analyze/AbstractAnalyze.h>
#include <modules/ModuleInfo.h>
#include <modules/input/BinaryInput.h>
#include <modules/input/PcapInput.h>
#include <modules/output/DatabaseOutput.h>
#include <modules/output/ConsoleOutput.h>
#include <modules/analyze/SyscallAnalyze.h>
#include <modules/analyze/LoopDetector.h>
#include <modules/analyze/GraphHash.h>

class ModuleManager {
public:
	virtual ~ModuleManager();
    static ModuleManager *instance()
	{
        static ModuleManager obj;
        return &obj;
	}

    QMap<QString, AbstractInput *> *input();
    QMap<QString, AbstractOutput *> *output();
    QMap<QString, AbstractAnalyze *> *analyze();

    QList<ModuleInfo *> listInput();
    QList<ModuleInfo *> listOutput();
    QList<ModuleInfo *> listAnalyze();

private:
	ModuleManager();

	void loadInput();
	void loadOutput();
	void loadAnalyze();

	void removeInput();
	void removeOutput();
	void removeAnalyze();

    QMap<QString, AbstractInput *> m_inputModules;
    QMap<QString, AbstractOutput *> m_outputModules;
    QMap<QString, AbstractAnalyze *> m_analyzeModules;
};

#endif /* MODULEMANAGER_H_ */
