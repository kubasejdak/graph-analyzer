/*
 *  Filename	: ExportTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 21-07-2013
 */

#include "ExportTask.h"

#include <string>
#include <list>
#include <algorithm>
#include <cstring>
#include <QDomElement>

#include <tasks/ITask.h>
#include <tasks/group/modules/ModulesManager.h>
#include <tasks/group/modules/input/IInput.h>
#include <tasks/analyze/modules/ModulesManager.h>
#include <tasks/analyze/modules/input/IInput.h>
#include <tasks/export/modules/ModulesManager.h>
#include <tasks/export/modules/output/IOutput.h>
#include <utils/SystemLogger.h>

using namespace std;

ExportTask::ExportTask()
{
	m_exportedSamples = 0;
	m_scheduledSamples = 0;

	m_type = "export";
	m_displayTraitName = "exported samples";
	m_displayTraitValue = "0";

	m_traits["samples"] = "0";

	loadModules();
}

void ExportTask::loadModules()
{
	m_inputMods = Group::ModulesManager::instance()->input();
	m_codeInputMods = Analyze::ModulesManager::instance()->input();
	m_outputMods = Export::ModulesManager::instance()->output();
}

bool ExportTask::performTask()
{
	SystemLogger::instance()->setStatus("export task");
	LOG("starting task: [%s], m_id: [%d]\n", m_type.c_str(), m_id);

	// gather all samples that match user criteria to be exported
	if(load() == false) {
		LOG_ERROR("loading samples failed\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	LOG("loaded samples: [%d]\n", m_samples.size());
	m_scheduledSamples = m_samples.size();

	// export results
	while(m_samples.size() > 0) {
		ExploitSampleHandle s = m_samples.front();
		m_samples.pop_front();

		if(exportResults(s) == false) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		++m_exportedSamples;
		updateStatus();
		SystemLogger::instance()->exportStatus(this);
	}

	// summarize
	LOG("=====================================================================================\n");
	LOG("=                                    FINISHED TASK                                   \n");
	LOG("= name             : %s\n", m_name.c_str());
	LOG("= exported samples : %d\n", m_exportedSamples);
	LOG("= errors           : %d\n", m_errors);
	LOG("=====================================================================================\n");

    m_progress = 100;
    SystemLogger::instance()->exportStatus(this);
	SystemLogger::instance()->setStatus("idle");

	LOG("SUCCESS\n\n");
	return true;
}

void ExportTask::updateStatus()
{
	// update general values
	if(m_scheduledSamples == 0)
		m_progress = 0;
	else
		m_progress = ((m_exportedSamples) * 100) / m_scheduledSamples;

	m_displayTraitValue = Toolbox::itos(m_exportedSamples);

	m_workTime = QTime(0, 0).addMSecs(m_timer.elapsed());

	// update traits
	m_traits["samples"] = to_string(m_exportedSamples);
}

bool ExportTask::readConfigXML(QDomElement taskNode)
{
	LOG("reading export task\n");

	m_name = m_xmlParser.child(taskNode, "Name").attribute("val").toStdString();
	LOG("name: [%s]\n", m_name.c_str());

	// files
	LOG("collecting files to export\n");
	QDomElement file = m_xmlParser.child(taskNode, "File");
	while(file.isNull() == false) {
		if(file.attribute("source") == "local") {
			m_taskFiles.push_back(file.attribute("path").toStdString());
            LOG("file [local]: %s\n", file.attribute("path").toStdString().c_str());
		}
		else {
            LOG("file [remote]: %s\n", file.attribute("path").toStdString().c_str());
			LOG("NOT SUPPORTED, SKIPPING\n");
		}

		file = file.nextSiblingElement("File");
	}

	// export directory
	m_exportDir = m_xmlParser.child(taskNode, "ExportDir").attribute("path").toStdString();
	LOG("export directory: [%s]\n", m_exportDir.c_str());

	// dates
	m_from = QDate::fromString(m_xmlParser.child(taskNode, "From").attribute("date"), "yyyy-MM-dd");
	LOG("from: [%s]\n", m_from.toString("yyyy-MM-dd").toStdString().c_str());
	m_until = QDate::fromString(m_xmlParser.child(taskNode, "Until").attribute("date"), "yyyy-MM-dd");
	LOG("until: [%s]\n", m_until.toString("yyyy-MM-dd").toStdString().c_str());

	// input strategy
	QDomElement in = m_xmlParser.child(taskNode, "Input");
	if(in.isNull() == false) {
		m_inputStrategy = in.attribute("val").toStdString();
		LOG("input strategy: [%s]\n", in.attribute("val").toStdString().c_str());
	}

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

int ExportTask::load()
{
	Group::InputMap::iterator it;
	bool moduleFound = false;

	for(it = m_inputMods->begin(); it != m_inputMods->end(); ++it) {
		if(m_inputStrategy == it->second->type()) {
			moduleFound = true;
			bool status = it->second->loadInput(this, &m_samples);
			if(status == false) {
				SystemLogger::instance()->setError("loading samples failed");
				LOG_ERROR("FAILURE\n\n");
				return false;
			}

			break;
		}
	}

	if(moduleFound == false) {
		LOG_ERROR("no appropriate input module\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	// load code for each sample
	LOG("loading code for each sample\n");
	bool ret = loadCode();

	if(ret == false) {
		LOG_ERROR("failed to load code\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool ExportTask::loadCode()
{
	SampleList::iterator sampleIt;
	for(sampleIt = m_samples.begin(); sampleIt != m_samples.end(); ++sampleIt) {
		ExploitSampleHandle sample = *sampleIt;

		// if this sample was already loaded
		if(sample->code() != NULL)
			continue;

		// load code using input modules
		SampleList q;
		bool moduleFound = false;

		Analyze::InputMap::iterator it;
		for(it = m_codeInputMods->begin(); it != m_codeInputMods->end(); ++it) {
			if(sample->info()->fileType() == it->second->type()) {
				moduleFound = true;
				bool ok = it->second->loadInput(sample->info()->extractedFrom(), &q);
				if(!ok) {
					SystemLogger::instance()->setError("loading file failed");
					LOG_ERROR("FAILURE\n\n");
					return false;
				}

				// get codes for all samples from this list that are within this task
				while(q.empty() == false) {
					ExploitSampleHandle h = q.front();
					q.pop_front();

					ExploitSampleHandle f = findSample(h->info()->name(), h->info()->extractedFrom());
					if(f != NULL) {
						// allocate buffer for code
						int size = h->info()->fileSize();
						char *buffer = new char[size];
						LOG("allocating code buffer for sample, size: [%d]\n", size);

						// copy code from
						memcpy(buffer, (char *) h->code(), size);
						f->setCode((byte_t *) buffer);
					}
				}

				break;
			}
		}

		if(moduleFound == false) {
			LOG_ERROR("no appropriate input module found to load sample code\n");
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	return true;
}

bool ExportTask::exportResults(ExploitSampleHandle s)
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

		bool status = (*m_outputMods)[exportStrategy]->exportOutput(s, m_exportDir);
		if(status == false) {
			LOG_ERROR("failed to export sample [%s] with strategy: [%s]\n", s->info()->name().c_str(), exportStrategy.c_str());
			++m_errors;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}

ExploitSampleHandle ExportTask::findSample(std::string name, std::string extractedFrom)
{
	SampleList::iterator it;
	for(it = m_samples.begin(); it != m_samples.end(); ++it) {
		ExploitSampleHandle s = *it;
		if(s->info()->name() == name && s->info()->extractedFrom() == extractedFrom)
			return s;
	}

	return NULL;
}
