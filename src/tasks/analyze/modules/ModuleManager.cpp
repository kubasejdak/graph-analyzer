/*
 *  Filename	: ModuleManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "ModuleManager.h"

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

ModuleManager::~ModuleManager()
{
	removeInput();
	removeOutput();
	removeAnalyze();
}

void ModuleManager::loadInput()
{
	// BinaryInput
	BinaryInput *binaryInput = new BinaryInput();
    m_inputModules[binaryInput->name()] = binaryInput;

	// BinaryInput
	PcapInput *pcapInput = new PcapInput();
    m_inputModules[pcapInput->name()] = pcapInput;
}

void ModuleManager::loadOutput()
{
	// DatabaseOutput
	DatabaseOutput *databaseOutput = new DatabaseOutput();
    m_outputModules[databaseOutput->name()] = databaseOutput;

	// DatabaseOutput
	ConsoleOutput *consoleOutput = new ConsoleOutput();
    m_outputModules[consoleOutput->name()] = consoleOutput;
}

void ModuleManager::loadAnalyze()
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

InputMap *ModuleManager::input()
{
	loadInput();
    return &m_inputModules;
}

OutputMap *ModuleManager::output()
{
	loadOutput();
    return &m_outputModules;
}

AnalyzeMap *ModuleManager::analyze()
{
	loadAnalyze();
    return &m_analyzeModules;
}

void ModuleManager::removeInput()
{
	InputMap::iterator it;
    for(it = m_inputModules.begin(); it != m_inputModules.end(); ++it)
		delete it->second;

    m_inputModules.clear();
}

void ModuleManager::removeOutput()
{
	OutputMap::iterator it;
    for(it = m_outputModules.begin(); it != m_outputModules.end(); ++it)
		delete it->second;

    m_outputModules.clear();
}

void ModuleManager::removeAnalyze()
{
	AnalyzeMap::iterator it;
    for(it = m_analyzeModules.begin(); it != m_analyzeModules.end(); ++it)
		delete it->second;

    m_analyzeModules.clear();
}
