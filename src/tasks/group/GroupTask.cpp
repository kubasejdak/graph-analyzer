/*
 *  Filename	: GroupTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "GroupTask.h"

#include <list>
#include <string>
#include <sstream>
#include <QDate>
#include <QDomElement>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>
#include <tasks/ITask.h>
#include <tasks/group/GroupManager.h>
#include <tasks/analyze/modules/ModuleManager.h>
#include <tasks/analyze/modules/analyze/IAnalyze.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>


using namespace std;

GroupTask::GroupTask()
{
    m_override = false;
    m_algorithm = "basic";

    m_type = "group";
    m_traitName = "groups";
    m_traitValue = "0";
	m_foundGroups = 0;
	m_groupedSamples = 0;
}

bool GroupTask::performTask()
{
	SystemLogger::instance()->setStatus("group task");
	LOG("starting task: [group], m_id: [%d]\n", m_id);

    if(collectTaskSamples() == false) {
        return false;
    }

	// TODO: implement

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
    m_override = m_xmlParser.child(taskNode, "Override").attribute("val") == "true" ? true : false;
    LOG("override: [%s]\n", (m_override) ? "true" : "false");

    /* files */
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

    m_from = QDate::fromString(m_xmlParser.child(taskNode, "From").attribute("date"), "yyyy-MM-dd");
    LOG("from: [%s]\n", m_from.toString("yyyy-MM-dd").toStdString().c_str());
    m_until = QDate::fromString(m_xmlParser.child(taskNode, "Until").attribute("date"), "yyyy-MM-dd");
    LOG("until: [%s]\n", m_until.toString("yyyy-MM-dd").toStdString().c_str());
    m_algorithm = m_xmlParser.child(taskNode, "Algorithm").attribute("name").toStdString();
	LOG("algorithm: [%s]\n", m_algorithm.c_str());

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

bool GroupTask::collectTaskSamples()
{
	// select all sample
	stringstream ss;
	ss << "SELECT * FROM analyze_sample WHERE " << "capture_date >= '" << m_from.toString("yyyy-MM-dd").toStdString() << "' AND "
												<< "capture_date <= '" << m_until.toString("yyyy-MM-dd").toStdString() << "'";

	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare(ss.str().c_str());
	if(!DatabaseManager::instance()->exec(&selectQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	AnalyzeMap *anaMods = ModuleManager::instance()->analyze();
	AnalyzeMap::iterator anaIt;
	ExploitSampleHandle sample;
	while(selectQuery.next()) {
		int sampleId = selectQuery.record().value("id").toInt();
		sample = ExploitSampleHandle(new ExploitSample());
		sample->info()->setName(selectQuery.record().value("name").toString().toStdString());
		sample->info()->setExtractedFrom(selectQuery.record().value("extracted_from").toString().toStdString());
		sample->info()->setGraphName(selectQuery.record().value("graph_name").toString().toStdString());
		sample->info()->setSize(selectQuery.record().value("size").toInt());
		sample->info()->setFileType(selectQuery.record().value("file_type").toString().toStdString());
		sample->info()->setFileSize(selectQuery.record().value("file_size").toInt());
		sample->info()->setCodeOffset(selectQuery.record().value("shellcode_offset").toInt());

		// if task files not empty, then check if sample matches criteria
		if(m_taskFiles.empty() == false) {
			bool pathFound = false;
			for(string path : m_taskFiles) {
				if(sample->info()->extractedFrom().find(path) != string::npos) {
					pathFound = true;
					break;
				}
			}

			if(pathFound == false)
				continue;
		}

		// get analyze specific information about current sample
		bool status;
		for(anaIt = anaMods->begin(); anaIt != anaMods->end(); ++anaIt) {
			status = anaIt->second->importFromDatabase(sample, sampleId);
			if(status == false) {
				LOG_ERROR("FAILURE\n\n");
				return false;
			}
		}

		m_samples.push_back(sample);
	}

	LOG("SUCCESS\n\n");
	return true;
}
