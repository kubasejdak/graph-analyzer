/*
 *  Filename	: ModuleManager.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 21-07-2013
 */

#include "ModulesManager.h"

#include <map>
#include <string>

#include <tasks/export/modules/output/SampleExportOutput.h>
#include <tasks/export/modules/output/IOutput.h>

using namespace std;
using namespace Export;

ModulesManager::~ModulesManager()
{
	removeOutput();
}

void ModulesManager::loadOutput()
{
	// SampleExportOutput
	SampleExportOutput *sampleExportOutput = new SampleExportOutput();
	m_outputModules[sampleExportOutput->name()] = sampleExportOutput;
}

OutputMap *ModulesManager::output()
{
	loadOutput();
    return &m_outputModules;
}

void ModulesManager::removeOutput()
{
	OutputMap::iterator it;
    for(it = m_outputModules.begin(); it != m_outputModules.end(); ++it)
		delete it->second;

    m_outputModules.clear();
}
