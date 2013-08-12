/*
 *  Filename	: ModuleManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "ModulesManager.h"

#include <map>
#include <string>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <tasks/group/modules/output/IOutput.h>
#include <tasks/group/modules/input/IInput.h>
#include <tasks/group/modules/algorithms/SymetricSymilarity.h>
#include <tasks/group/modules/output/ConsoleOutput.h>
#include <tasks/group/modules/output/DatabaseOutput.h>
#include <tasks/group/modules/input/DatabaseInput.h>

using namespace std;
using namespace Group;

ModulesManager::~ModulesManager()
{
	removeInput();
	removeOutput();
    removeAlgorithm();
}

void ModulesManager::loadInput()
{
	// DatabaseInput
	DatabaseInput *databaseInput = new DatabaseInput();
	m_inputModules[databaseInput->name()] = databaseInput;
}

void ModulesManager::loadOutput()
{
	// ConsoleOutput
	ConsoleOutput *consoleOutput = new ConsoleOutput();
	m_outputModules[consoleOutput->name()] = consoleOutput;

	// DatabaseOutput
	DatabaseOutput *databaseOutput = new DatabaseOutput();
	m_outputModules[databaseOutput->name()] = databaseOutput;
}

void ModulesManager::loadAlgorithm()
{
	// SymetricProbability
	SymetricSimilarity *symetricProbability = new SymetricSimilarity();
    m_algorithmModules[symetricProbability->name()] = symetricProbability;
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

AlgorithmMap *ModulesManager::algorithm()
{
    loadAlgorithm();
    return &m_algorithmModules;
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

void ModulesManager::removeAlgorithm()
{
    AlgorithmMap::iterator it;
    for(it = m_algorithmModules.begin(); it != m_algorithmModules.end(); ++it)
		delete it->second;

    m_algorithmModules.clear();
}
