/*
 * Filename		: AnalysisSystem.cpp
 * Author		: Kuba Sejdak
 * Created on	: 21-06-2012
 */

#include "AnalysisSystem.h"

#include <core/ExploitSample.h>
#include <utils/SystemLogger.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>

using namespace std;
using namespace Analyze;

AnalysisSystem::AnalysisSystem()
{
    m_sample = NULL;
	loadModules();
}

void AnalysisSystem::loadModules()
{
	m_analyzeMods = ModuleManager::instance()->analyze();
}

void AnalysisSystem::loadSample(ExploitSampleHandle sample)
{
    m_sample = sample;
}

bool AnalysisSystem::analyze()
{
    if(!m_sample) {
        LOG_ERROR("m_sample: [null]\n");
        LOG_ERROR("FAILURE\n\n");
		return false;
    }

	if(!m_sample->info()->isExploitPresent()) {
        m_sample = NULL;
        LOG("no exploit found, nothing to analyze, returning\n");
        LOG("SUCCESS\n\n");
		return true;
	}

	bool status;
	AnalyzeMap::iterator it;
	for(it = m_analyzeMods->begin(); it != m_analyzeMods->end(); ++it) {
		status = it->second->perform(m_sample);
        if(!status) {
			LOG_ERROR("analyze module failed: [%s]\n", it->first.c_str());
            LOG_ERROR("FAILURE\n\n");
            return false;
        }
	}

    m_sample = NULL;
    LOG("SUCCESS\n\n");
	return true;
}
