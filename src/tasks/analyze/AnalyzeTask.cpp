/*
 *  Filename	: AnalyzeTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#include "AnalyzeTask.h"
#include <utils/FileTypeAnalyzer.h>
#include <utils/SystemLogger.h>
#include <core/Options.h>

AnalyzeTask::AnalyzeTask(int id) : ITask(id)
{
	m_override = false;
	m_scheduledFiles = 0;
	m_loadedFiles = 0;
	m_analyzedSamples = 0;
	m_detectedExploits = 0;

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

	/* read files to analyze */
	if(readConfigXML() == false) {
		SystemLogger::instance()->setError("cannot parse tasks file");
		SystemLogger::instance()->setStatus("idle");
		LOG_ERROR("cannot parse tasks file\n");
		LOG_ERROR("FAILUR\n\n");
		return false;
	}

	/* analyze all task files */
	LOG("analyzing task files\n");
	while(!m_taskFiles.isEmpty()) {
		QString currentFile = m_taskFiles.front();
		m_taskFiles.pop_front();
		LOG("processing file: [%s]\n", currentFile.toStdString().c_str());

		/* load */
		LOG("loading\n");
		int loadError = load(currentFile);
		if(loadError) {
			if(loadError == -1) {
				LOG_ERROR("loading file [%s] -> [%s]\n", currentFile.toStdString().c_str(), SystemLogger::instance()->error().toStdString().c_str());
                SystemLogger::instance()->setError("cannot parse tasks file");
				++m_errors;
			}
			else
				LOG_ERROR("loading file [%s] -> [no appropriate input module]\n", currentFile.toStdString().c_str());

			continue;
		}

		ExploitSample *s;
		while(!m_samples.isEmpty()) {
			s = m_samples.front();
			m_samples.pop_front();
			LOG("processing sample: [%s]\n", s->info()->name().toStdString().c_str());

			/* emulate */
			LOG("emulating\n");
			if(emulate(s) == false) {
				LOG_ERROR("emulating [%s] -> [%s]\n", currentFile.toStdString().c_str(), SystemLogger::instance()->error().toStdString().c_str());
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

			/* analyze graph */
			LOG("analyzing\n");
			if(analyze(s) == false) {
				LOG_ERROR("analyzing [%s] -> [%s]\n", currentFile.toStdString().c_str(), SystemLogger::instance()->error().toStdString().c_str());
				++m_errors;
				goto cleanup;
			}

			/* export results */
			LOG("exporting results\n");
			if(exportResults(s) == false) {
				LOG_ERROR("output for [%s] -> [%s]\n", currentFile.toStdString().c_str(), SystemLogger::instance()->error().toStdString().c_str());
				++m_errors;
				goto cleanup;
			}

			++m_detectedExploits;

cleanup:
			/* clean up */
			delete s;
			LOG("sample analyzing finished\n");
			++m_analyzedSamples;

			/* export status */
			SystemLogger::instance()->exportStatus(this);
		}
		LOG("file analyzing finished\n");
	} /* while */

	/* summarize */
	LOG("FINISHED: found %d exploit(s) in %d sample(s) extracted from %d file(s)!\n", m_detectedExploits, m_analyzedSamples, m_loadedFiles);
	if(m_errors)
		LOG("some ERRORS occured, errorCounter: [%d]\n", m_errors);
	else
		LOG("no errors occured\n");

	SystemLogger::instance()->setStatus("idle");
	LOG("SUCCESS\n\n");
	return true;
}

bool AnalyzeTask::readConfigXML()
{
	if(!m_xmlParser.open(TASKS_FILE)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	if(!m_xmlParser.hasRoot(ROOT_NODE)) {
		m_xmlParser.close();
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	QDomElement taskElement = m_xmlParser.root(ROOT_NODE);
	int taskId = 0;
	while(taskId != m_id) {
		taskElement = taskElement.nextSiblingElement(ROOT_NODE);
		++taskId;
	}

	m_override = m_xmlParser.child(taskElement, "Override").attribute("val") == "true" ? true : false;

	/* files */
	LOG("collecting files to analyze\n");
	while(m_xmlParser.hasChild(taskElement, "File")) {
		QDomElement file = m_xmlParser.child(taskElement, "File");
		if(file.attribute("source") == "local") {
			addScheduledFile(file.text());
			LOG("[local]: %s\n", file.text().toStdString().c_str());
		}
		else {
			LOG("[remote]: %s\n", file.text().toStdString().c_str());
			LOG("NOT SUPPORTED, SKIPPING\n");
		}

		++m_scheduledFiles;
		m_xmlParser.removeChild(taskElement, file);
	}

    /* output strategies */
	while(m_xmlParser.hasChild(taskElement, "Output")) {
		QDomElement out = m_xmlParser.child(taskElement, "Output");
        m_exportStrategies.push_back(out.attribute("val"));
		LOG("added output strategy: [%s]\n", out.attribute("val").toStdString().c_str());
		m_xmlParser.removeChild(taskElement, out);
	}

	m_xmlParser.close();
	LOG("SUCCESS\n\n");
	return true;
}

void AnalyzeTask::addScheduledFile(QString filename)
{
	if(filename.isEmpty())
		return;

	int addCounter = 0;
	/* check if directory */
	if(QDir(filename).exists()) {
		QDirIterator it(filename, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while(it.hasNext()) {
			QString entryName = it.next();
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

	LOG("added [%d] file(s) extracted from [%s]\n", addCounter, filename.toStdString().c_str());
	LOG("SUCCESS\n\n");
}

int AnalyzeTask::load(QString filename)
{
	SampleContainer q;
	ExploitSample *s;
	bool moduleFound = false;

	FileTypeAnalyzer fileAnalyzer;
	QString fileType = fileAnalyzer.analyze(filename);
	LOG("fileType: [%s]\n", fileType.toStdString().c_str());

	InputMap::iterator it;
	for(it = m_inputMods->begin(); it != m_inputMods->end(); ++it) {
		if(fileType == it.value()->type()) {
			moduleFound = true;
			bool ok = it.value()->loadInput(filename, &q);
			if(!ok) {
				SystemLogger::instance()->setError("loading file failed");
				LOG_ERROR("FAILURE\n\n");
				return -1;
			}

			/* process all returned samples */
			while(!q.isEmpty()) {
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

bool AnalyzeTask::emulate(ExploitSample *s)
{
	m_emulationSystem.loadSample(s);
	bool ret = m_emulationSystem.emulate();

	if(!ret) {
		SystemLogger::instance()->setError("general emulation error");
		LOG_ERROR("general emulation error\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool AnalyzeTask::analyze(ExploitSample *s)
{
	m_analysisSystem.loadSample(s);
	bool ret = m_analysisSystem.analyze();

	if(!ret) {
		SystemLogger::instance()->setError("general analyzing error");
		LOG_ERROR("general analyzing error\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool AnalyzeTask::exportResults(ExploitSample *s)
{
    for(int i = 0; i < m_exportStrategies.size(); ++i) {
        QString exportStrategy = m_exportStrategies.at(i);
		LOG("exporting using strategy: [%s]\n", exportStrategy.toStdString().c_str());
		if(!m_outputMods->contains(exportStrategy)) {
			LOG_ERROR("output strategy not supported: [%s]", exportStrategy.toStdString().c_str());
			++m_errors;
			continue;
		}

		bool status = (*m_outputMods)[exportStrategy]->exportOutput(s);
		if(status == false) {
			LOG_ERROR("failed to export sample [%s] with strategy: [%s]", s->info()->name().toStdString().c_str(), exportStrategy.toStdString().c_str());
			++m_errors;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
