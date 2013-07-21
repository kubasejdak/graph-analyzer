/*
 *  Filename	: ModuleManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "ModulesManager.h"

#include <map>
#include <string>

#include <tasks/analyze/modules/input/BinaryInput.h>
#include <tasks/analyze/modules/input/PcapInput.h>
#include <tasks/analyze/modules/output/DatabaseOutput.h>
#include <tasks/analyze/modules/output/ConsoleOutput.h>
#include <tasks/analyze/modules/analyze/SyscallAnalyze.h>
#include <tasks/analyze/modules/analyze/LoopDetector.h>
#include <tasks/analyze/modules/analyze/GraphHash.h>
#include <tasks/analyze/modules/input/IInput.h>
#include <tasks/analyze/modules/output/IOutput.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>

using namespace std;
using namespace Analyze;

ModulesManager::~ModulesManager()
{
	removeInput();
	removeOutput();
	removeAnalyze();
}

void ModulesManager::loadInput()
{
	// BinaryInput
	BinaryInput *binaryInput = new BinaryInput();
    m_inputModules[binaryInput->name()] = binaryInput;

	// BinaryInput
	PcapInput *pcapInput = new PcapInput();
    m_inputModules[pcapInput->name()] = pcapInput;
}

void ModulesManager::loadOutput()
{
	// DatabaseOutput
	DatabaseOutput *databaseOutput = new DatabaseOutput();
    m_outputModules[databaseOutput->name()] = databaseOutput;

	// ConsoleOutput
	ConsoleOutput *consoleOutput = new ConsoleOutput();
    m_outputModules[consoleOutput->name()] = consoleOutput;
}

void ModulesManager::loadAnalyze()
{
	// SyscallAnalyze
	SyscallAnalyze *syscallAnalyze = new SyscallAnalyze();
    m_analyzeModules[syscallAnalyze->name()] = syscallAnalyze;

	// GraphHash
	GraphHash *graphHash = new GraphHash();
    m_analyzeModules[graphHash->name()] = graphHash;

	// LoopDetector
	LoopDetector *loopDetector = new LoopDetector();
    m_analyzeModules[loopDetector->name()] = loopDetector;
}

InputMap *ModulesManager::input()
{
	loadInput();
    return &m_inputModules;
}

OutputMap *ModulesManager::output()
{
	loadOutput();
    return &m_outputModules;
}

AnalyzeMap *ModulesManager::analyze()
{
	loadAnalyze();
    return &m_analyzeModules;
}

void ModulesManager::removeInput()
{
	InputMap::iterator it;
    for(it = m_inputModules.begin(); it != m_inputModules.end(); ++it)
		delete it->second;

    m_inputModules.clear();
}

void ModulesManager::removeOutput()
{
	OutputMap::iterator it;
    for(it = m_outputModules.begin(); it != m_outputModules.end(); ++it)
		delete it->second;

    m_outputModules.clear();
}

void ModulesManager::removeAnalyze()
{
	AnalyzeMap::iterator it;
    for(it = m_analyzeModules.begin(); it != m_analyzeModules.end(); ++it)
		delete it->second;

    m_analyzeModules.clear();
}
