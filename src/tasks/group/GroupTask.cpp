/*
 *  Filename	: GroupTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "GroupTask.h"

#include <list>
#include <string>
#include <sstream>
#include <QDomElement>

#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/ITask.h>
#include <tasks/group/GroupManager.h>
#include <tasks/group/modules/algorithms/IAlgorithm.h>
#include <tasks/group/modules/output/IOutput.h>
#include <tasks/group/modules/input/IInput.h>
#include <tasks/group/modules/algorithms/AlgorithmContext.h>
#include <tasks/group/modules/ModulesManager.h>

using namespace std;

GroupTask::GroupTask()
{
    m_duplicate = false;
    m_algorithm = "SymetricProbability";
	m_inputStrategy = "database";

    m_type = "group";
    m_traitName = "groups";
    m_traitValue = "0";
	m_foundGroups = 0;
	m_groupedSamples = 0;
}

QDate GroupTask::from()
{
	return m_from;
}

QDate GroupTask::until()
{
	return m_until;
}

list<string> GroupTask::taskFiles()
{
	return m_taskFiles;
}

bool GroupTask::performTask()
{
	SystemLogger::instance()->setStatus("group task");
	LOG("starting task: [group], m_id: [%d]\n", m_id);

	// gather all samples that match user criteria to be grouped
	if(load() == false) {
        return false;
    }
	LOG("loaded samples: [%d]\n", m_samples.size());

    // get grouping algorithm
	Group::IAlgorithm *algorithm = (*Group::ModulesManager::instance()->algorithm())[m_algorithm];

	// for each sample try to find appropriate group for it
	for(ExploitSampleHandle sample : m_samples) {
		// if no groups found (first sample), create immediately group for it
		if(m_groupManager.count() == 0) {
			int groupId = m_groupManager.createGroup();
			m_groupManager.add(groupId, sample);

			++m_foundGroups;
			++m_groupedSamples;

			// export status
			updateStatus();
			SystemLogger::instance()->exportStatus(this);

			continue;
		}

		// for each existing group, compare its leader and current sample
		bool assign = false;
		for(int i = 0; i < m_groupManager.count(); ++i) {
			assign = algorithm->process(m_groupManager.leader(i), sample, m_context);

			if(assign == true) {
				m_groupManager.add(i, sample);
				break;
			}
		}

		// if not assigned to any group, create group for it
		if(assign == false) {
			int groupId = m_groupManager.createGroup();
			m_groupManager.add(groupId, sample);

			++m_foundGroups;
		}

		++m_groupedSamples;

		// export status
		updateStatus();
		LOG("progress: [%d]\n", m_progress);
		SystemLogger::instance()->exportStatus(this);
	}

	// export results
	for(int i = 0; i < m_groupManager.count(); ++i) {
		if(exportResults(m_groupManager.group(i)) == false) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}


	// summarize
	LOG("=====================================================================================\n");
	LOG("=                                    FINISHED TASK                                   \n");
	LOG("= name     : %s\n", m_name.c_str());
	LOG("= samples  : %d\n", m_samples.size());
	LOG("= groups   : %d\n", m_foundGroups);
	LOG("= errors   : %d\n", m_errors);
	LOG("=====================================================================================\n");

	m_progress = 100;
	SystemLogger::instance()->exportStatus(this);
	SystemLogger::instance()->setStatus("idle");

    LOG("SUCCESS\n\n");
	return true;
}

void GroupTask::updateStatus()
{
	if(m_samples.size() > 0)
		m_progress = (m_groupedSamples * 100) / m_samples.size();
	else
		m_progress = 100;

	m_traitValue = Toolbox::itos(m_foundGroups);

	m_workTime = QTime(0, 0).addMSecs(m_timer.elapsed());
}

bool GroupTask::readConfigXML(QDomElement taskNode)
{
    LOG("reading group task\n");

    m_name = m_xmlParser.child(taskNode, "Name").attribute("val").toStdString();
	LOG("name: [%s]\n", m_name.c_str());
	m_duplicate = m_xmlParser.child(taskNode, "Duplicate").attribute("val") == "true" ? true : false;
	LOG("duplicate: [%s]\n", (m_duplicate) ? "true" : "false");

	// files
    LOG("collecting files to analyze\n");
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

	// dates
    m_from = QDate::fromString(m_xmlParser.child(taskNode, "From").attribute("date"), "yyyy-MM-dd");
    LOG("from: [%s]\n", m_from.toString("yyyy-MM-dd").toStdString().c_str());
    m_until = QDate::fromString(m_xmlParser.child(taskNode, "Until").attribute("date"), "yyyy-MM-dd");
    LOG("until: [%s]\n", m_until.toString("yyyy-MM-dd").toStdString().c_str());

	// algorithm
	m_algorithm = m_xmlParser.child(taskNode, "Algorithm").attribute("name").toStdString();
	LOG("algorithm: [%s]\n", m_algorithm.c_str());

	// algorithm context
	QDomElement context = m_xmlParser.child(taskNode, "Context");
	while(context.isNull() == false) {
		m_context.setValue(context.attribute("name").toStdString(), context.attribute("val").toStdString());
		LOG("algorithm context: name: [%s], value: [%s]\n", context.attribute("name").toStdString().c_str(), context.attribute("val").toStdString().c_str());
		context = context.nextSiblingElement("Context");
	}

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

bool GroupTask::load()
{
	Group::InputMap *inputMods = Group::ModulesManager::instance()->input();
	Group::InputMap::iterator it;
	bool moduleFound = false;

	for(it = inputMods->begin(); it != inputMods->end(); ++it) {
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

	if(moduleFound) {
		LOG("SUCCESS\n\n");
		return true;
	}

	LOG_ERROR("no appropriate input module\n");
	LOG_ERROR("FAILURE\n\n");
	return false;
}

bool GroupTask::exportResults(ExploitGroupHandle g)
{
	Group::OutputMap *outputMods = Group::ModulesManager::instance()->output();

	list<string>::iterator it;
	for(it = m_exportStrategies.begin(); it != m_exportStrategies.end(); ++it) {
		string exportStrategy = *it;
		LOG("exporting using strategy: [%s]\n", exportStrategy.c_str());
		if(outputMods->find(exportStrategy) == outputMods->end()) {
			LOG_ERROR("output strategy not supported: [%s]\n", exportStrategy.c_str());
			++m_errors;
			continue;
		}

		bool status = (*outputMods)[exportStrategy]->exportOutput(g, m_id, m_duplicate);
		if(status == false) {
			LOG_ERROR("failed to export group with strategy: [%s]\n", exportStrategy.c_str());
			++m_errors;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
