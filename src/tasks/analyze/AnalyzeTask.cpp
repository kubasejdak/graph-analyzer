/*
 *  Filename	: AnalyzeTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#include "AnalyzeTask.h"

#include <string>
#include <list>
#include <QDomElement>
#include <QDir>
#include <QDirIterator>

#include <tasks/ITask.h>
#include <tasks/analyze/modules/output/IOutput.h>
#include <tasks/analyze/modules/input/IInput.h>
#include <tasks/analyze/AnalysisSystem.h>
#include <tasks/analyze/EmulationSystem.h>
#include <tasks/analyze/modules/ModuleManager.h>
#include <utils/FileTypeAnalyzer.h>
#include <utils/SystemLogger.h>
#include <utils/Toolbox.h>
#include <core/Options.h>
#include <core/ExploitSample.h>

using namespace std;

AnalyzeTask::AnalyzeTask()
{
	m_override = false;
	m_scheduledFiles = 0;
	m_loadedFiles = 0;
	m_analyzedSamples = 0;
	m_detectedExploits = 0;
	m_allTaskFiles = 0;

    m_type = "analyze";
	m_traitName = "exploits";
	m_traitValue = "0";

	loadModules();
}

void AnalyzeTask::loadModules()
{
	m_inputMods = ModuleManager::instance()->input();
	m_outputMods = ModuleManager::instance()->output();
}

bool AnalyzeTask::performTask()
{
	SystemLogger::instance()->setStatus("analyze task");
	LOG("starting task: [analyze], m_id: [%d]\n", m_id);

	// analyze all task files
	LOG("analyzing task files\n");
	while(m_taskFiles.empty() == false) {
		string currentFile = m_taskFiles.front();
		m_taskFiles.pop_front();
		LOG("processing file: [%s]\n", currentFile.c_str());

		// load
		LOG("loading\n");
		int loadError = load(currentFile);
		if(loadError) {
			if(loadError == -1) {
				LOG_ERROR("loading file [%s] -> [%s]\n", currentFile.c_str(), SystemLogger::instance()->error().c_str());
				SystemLogger::instance()->setError("cannot load file");
				++m_errors;
			}
			else
				LOG_ERROR("loading file [%s] -> [no appropriate input module]\n", currentFile.c_str());

			continue;
		}

		ExploitSampleHandle s;
		while(m_samples.empty() == false) {
			s = m_samples.front();
			m_samples.pop_front();
			LOG("processing sample: [%s]\n", s->info()->name().c_str());

			// emulate
			LOG("emulating\n");
			if(emulate(s) == false) {
				LOG_ERROR("emulating [%s] -> [%s]\n", currentFile.c_str(), SystemLogger::instance()->error().c_str());
				++m_errors;
				goto cleanup;
			}

			if(s->info()->isExploitPresent() == false) {
				LOG("no exploit found, skipping\n");
				goto cleanup;
			}
			if(s->info()->isBroken()) {
				LOG("broken sample, skipping due to skipBrokenSamples: [true]\n");
				goto cleanup;
			}

			// analyze graph
			LOG("analyzing\n");
			if(analyze(s) == false) {
				LOG_ERROR("analyzing [%s] -> [%s]\n", currentFile.c_str(), SystemLogger::instance()->error().c_str());
				++m_errors;
				goto cleanup;
			}

			// export results
			LOG("exporting results\n");
			if(exportResults(s) == false) {
				LOG_ERROR("output for [%s] -> [%s]\n", currentFile.c_str(), SystemLogger::instance()->error().c_str());
				++m_errors;
				goto cleanup;
			}

			++m_detectedExploits;

cleanup:
			// clean up
			s.reset();
			LOG("sample analyzing finished\n");
			++m_analyzedSamples;

			// export status
			updateStatus();
			SystemLogger::instance()->exportStatus(this);
		}
		LOG("file analyzing finished\n");
	} // while

	// summarize
	LOG("=====================================================================================\n");
	LOG("=                                    FINISHED TASK                                   \n");
	LOG("= name     : %s\n", m_name.c_str());
	LOG("= exploits : %d\n", m_detectedExploits);
	LOG("= samples  : %d\n", m_analyzedSamples);
	LOG("= files    : %d\n", m_loadedFiles);
	LOG("= errors   : %d\n", m_errors);
	LOG("=====================================================================================\n");

    m_progress = 100;
    SystemLogger::instance()->exportStatus(this);

	SystemLogger::instance()->setStatus("idle");
	LOG("SUCCESS\n\n");
	return true;
}

void AnalyzeTask::updateStatus()
{
	if(m_allTaskFiles > 0)
		m_progress = ((m_allTaskFiles - m_taskFiles.size()) * 100) / m_allTaskFiles;
	else
		m_progress = 100;

	m_traitValue = Toolbox::itos(m_detectedExploits);

	m_workTime = QTime(0, 0).addMSecs(m_timer.elapsed());	
}

bool AnalyzeTask::readConfigXML(QDomElement taskNode)
{
    LOG("reading analyze task\n");

	m_name = m_xmlParser.child(taskNode, "Name").attribute("val").toStdString();
	LOG("name: [%s]\n", m_name.c_str());
	m_override = m_xmlParser.child(taskNode, "Override").attribute("val") == "true" ? true : false;
    LOG("override: [%s]\n", (m_override) ? "true" : "false");

	// files
	LOG("collecting files to analyze\n");
	QDomElement file = m_xmlParser.child(taskNode, "File");
	while(file.isNull() == false) {
		if(file.attribute("source") == "local") {
			addScheduledFile(file.attribute("path").toStdString());
            LOG("file [local]: %s\n", file.attribute("path").toStdString().c_str());
		}
		else {
            LOG("file [remote]: %s\n", file.attribute("path").toStdString().c_str());
			LOG("NOT SUPPORTED, SKIPPING\n");
		}

		++m_scheduledFiles;
		file = file.nextSiblingElement("File");
	}
	m_allTaskFiles = m_taskFiles.size();

	// output strategies
	QDomElement out = m_xmlParser.child(taskNode, "Output");
	while(out.isNull() == false) {
		m_exportStrategies.push_back(out.attribute("val").toStdString());
        LOG("output strategy: [%s]\n", out.attribute("val").toStdString().c_str());
		out = out.nextSiblingElement("Output");
	}

    LOG("SUCCESS\n\n");
	return true;
}

void AnalyzeTask::addScheduledFile(string filename)
{
	if(filename.empty())
		return;

	//filename = filename.trimmed();
	int addCounter = 0;
	// check if directory
	if(QDir(filename.c_str()).exists()) {
		QDirIterator it(filename.c_str(), QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while(it.hasNext()) {
			string entryName = it.next().toStdString();
			m_taskFiles.push_back(entryName);
			++m_loadedFiles;
			++addCounter;
		}
	}
	else {
		m_taskFiles.push_back(filename);
		++m_loadedFiles;
		++addCounter;
	}

	LOG("added [%d] file(s) extracted from [%s]\n", addCounter, filename.c_str());
	LOG("SUCCESS\n\n");
}

int AnalyzeTask::load(string filename)
{
	SampleList q;
	ExploitSampleHandle s;
	bool moduleFound = false;

	FileTypeAnalyzer fileAnalyzer;
	string fileType = fileAnalyzer.analyze(filename);
	LOG("fileType: [%s]\n", fileType.c_str());

	InputMap::iterator it;
	for(it = m_inputMods->begin(); it != m_inputMods->end(); ++it) {
		if(fileType == it->second->type()) {
			moduleFound = true;
			bool ok = it->second->loadInput(filename, &q);
			if(!ok) {
				SystemLogger::instance()->setError("loading file failed");
				LOG_ERROR("FAILURE\n\n");
				return -1;
			}

			/* process all returned samples */
			while(q.empty() == false) {
				s = q.front();
				q.pop_front();
				m_samples.push_back(s);
			}

			break;
		}
	}

	if(moduleFound) {
		LOG("SUCCESS\n\n");
		return 0;
	}

	LOG_ERROR("no appropriate input module\n");
	LOG_ERROR("FAILURE\n\n");
	return 1;
}

bool AnalyzeTask::emulate(ExploitSampleHandle s)
{
	m_emulationSystem.loadSample(s);
	bool ret = m_emulationSystem.emulate();

	if(ret == false) {
		SystemLogger::instance()->setError("general emulation error");
		LOG_ERROR("general emulation error\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool AnalyzeTask::analyze(ExploitSampleHandle s)
{
	m_analysisSystem.loadSample(s);
	bool ret = m_analysisSystem.analyze();

	if(ret == false) {
		SystemLogger::instance()->setError("general analyzing error");
		LOG_ERROR("general analyzing error\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool AnalyzeTask::exportResults(ExploitSampleHandle s)
{
	list<string>::iterator it;
	for(it = m_exportStrategies.begin(); it != m_exportStrategies.end(); ++it) {
		string exportStrategy = *it;
		LOG("exporting using strategy: [%s]\n", exportStrategy.c_str());
		if(m_outputMods->find(exportStrategy) == m_outputMods->end()) {
			LOG_ERROR("output strategy not supported: [%s]\n", exportStrategy.c_str());
			++m_errors;
			continue;
		}

        bool status = (*m_outputMods)[exportStrategy]->exportOutput(s, m_id);
		if(status == false) {
			LOG_ERROR("failed to export sample [%s] with strategy: [%s]\n", s->info()->name().c_str(), exportStrategy.c_str());
			++m_errors;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
