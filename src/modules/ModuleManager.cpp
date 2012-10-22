/*
 *  Filename	: ModuleManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "ModuleManager.h"

ModuleManager::ModuleManager()
{
}

ModuleManager::~ModuleManager()
{
	removeInput();
	removeOutput();
	removeAnalyze();
}

void ModuleManager::loadInput()
{
	/* BinaryInput */
	BinaryInput *binaryInput = new BinaryInput();
    m_inputModules[binaryInput->name()] = binaryInput;

	/* BinaryInput */
	PcapInput *pcapInput = new PcapInput();
    m_inputModules[pcapInput->name()] = pcapInput;
}

void ModuleManager::loadOutput()
{
	/* DatabaseOutput */
	DatabaseOutput *databaseOutput = new DatabaseOutput();
    m_outputModules[databaseOutput->name()] = databaseOutput;

	/* DatabaseOutput */
	ConsoleOutput *consoleOutput = new ConsoleOutput();
    m_outputModules[consoleOutput->name()] = consoleOutput;
}

void ModuleManager::loadAnalyze()
{
	/* SyscallAnalyze */
	SyscallAnalyze *syscallAnalyze = new SyscallAnalyze();
    m_analyzeModules[syscallAnalyze->name()] = syscallAnalyze;

	/* GraphHash */
	GraphHash *graphHash = new GraphHash();
    m_analyzeModules[graphHash->name()] = graphHash;

	/* LoopDetector */
	LoopDetector *loopDetector = new LoopDetector();
    m_analyzeModules[loopDetector->name()] = loopDetector;
}

QMap<QString, AbstractInput *> *ModuleManager::input()
{
	loadInput();
    return &m_inputModules;
}

QMap<QString, AbstractOutput *> *ModuleManager::output()
{
	loadOutput();
    return &m_outputModules;
}

QMap<QString, AbstractAnalyze *> *ModuleManager::analyze()
{
	loadAnalyze();
    return &m_analyzeModules;
}

QList<ModuleInfo *> ModuleManager::listInput()
{
    QList<ModuleInfo *> modList;
    QMap<QString, AbstractInput *>::iterator it;
    for(it = m_inputModules.begin(); it != m_inputModules.end(); ++it)
        modList.push_back(it.value()->moduleInfo());

	return modList;
}

QList<ModuleInfo *> ModuleManager::listOutput()
{
    QList<ModuleInfo *> modList;
    QMap<QString, AbstractOutput *>::iterator it;
    for(it = m_outputModules.begin(); it != m_outputModules.end(); ++it)
        modList.push_back(it.value()->moduleInfo());

	return modList;
}

QList<ModuleInfo *> ModuleManager::listAnalyze()
{
    QList<ModuleInfo *> modList;
    QMap<QString, AbstractAnalyze *>::iterator it;
    for(it = m_analyzeModules.begin(); it != m_analyzeModules.end(); ++it)
        modList.push_back(it.value()->moduleInfo());

	return modList;
}

void ModuleManager::removeInput()
{
    QMap<QString, AbstractInput *>::iterator it;
    for(it = m_inputModules.begin(); it != m_inputModules.end(); ++it)
        delete it.value();

    m_inputModules.clear();
}

void ModuleManager::removeOutput()
{
    QMap<QString, AbstractOutput *>::iterator it;
    for(it = m_outputModules.begin(); it != m_outputModules.end(); ++it)
        delete it.value();

    m_outputModules.clear();
}

void ModuleManager::removeAnalyze()
{
    QMap<QString, AbstractAnalyze *>::iterator it;
    for(it = m_analyzeModules.begin(); it != m_analyzeModules.end(); ++it)
        delete it.value();

    m_analyzeModules.clear();
}
