/*
 *  Filename	: ModuleManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "ModulesManager.h"

#include <map>
#include <string>

#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <tasks/group/modules/algorithms/SymetricProbability.h>

using namespace std;
using namespace Group;

ModuleManager::~ModuleManager()
{
	removeInput();
	removeOutput();
    removeAlgorithm();
}

void ModuleManager::loadInput()
{
}

void ModuleManager::loadOutput()
{
}

void ModuleManager::loadAlgorithm()
{
    // SymetricProbability
    SymetricProbability *symetricProbability = new SymetricProbability();
    m_algorithmModules[symetricProbability->name()] = symetricProbability;
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

AlgorithmMap *ModuleManager::algorithm()
{
    loadAlgorithm();
    return &m_algorithmModules;
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

void ModuleManager::removeAlgorithm()
{
    AlgorithmMap::iterator it;
    for(it = m_algorithmModules.begin(); it != m_algorithmModules.end(); ++it)
		delete it->second;

    m_algorithmModules.clear();
}
