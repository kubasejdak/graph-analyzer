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
    LOG("\n");
    if(!m_sample) {
        LOG_ERROR("m_sample: [null]\n");
        LOG_ERROR("FAILURE\n");
		return false;
    }

    if(!m_sample->info()->isShellcodePresent()) {
        m_sample = NULL;
        LOG("no exploit found, nothing to analyze, returning\n");
        LOG("SUCCESS\n");
		return true;
	}

	bool status;
    QMap<QString, AbstractAnalyze *>::iterator it;
    for(it = m_analyzeModules->begin(); it != m_analyzeModules->end(); ++it) {
        status = it.value()->perform(m_sample);
        if(!status) {
            LOG_ERROR("analyze module failed: [%s]\n", it.key().toStdString().c_str());
            LOG_ERROR("FAILURE\n");
            return false;
        }
	}

    m_sample = NULL;
    LOG("SUCCESS\n");
	return true;
}

void AnalysisSystem::loadModules()
{
    m_analyzeModules = ModuleManager::instance()->analyze();
}
