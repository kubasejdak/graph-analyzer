/*
 * Filename		: AnalysisSystem.cpp
 * Author		: Kuba Sejdak
 * Created on	: 21-06-2012
 */

#include "AnalysisSystem.h"

AnalysisSystem::AnalysisSystem()
{
    m_sample = NULL;
	loadModules();
}

AnalysisSystem::~AnalysisSystem()
{
}

void AnalysisSystem::loadSample(ShellcodeSample *sample)
{
    m_sample = sample;
}

bool AnalysisSystem::analyze()
{
    if(!m_sample)
		return false;

    if(!m_sample->info()->isShellcodePresent()) {
        m_sample = NULL;
		return true;
	}

	bool status;
    QMap<QString, AbstractAnalyze *>::iterator it;
    for(it = m_analyzeModules->begin(); it != m_analyzeModules->end(); ++it) {
        status = it.value()->perform(m_sample);
        if(!status)
            return false;
	}

    m_sample = NULL;
	return true;
}

void AnalysisSystem::loadModules()
{
    m_analyzeModules = ModuleManager::instance()->analyze();
}
