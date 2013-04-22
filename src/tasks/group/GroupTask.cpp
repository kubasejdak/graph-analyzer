/*
 *  Filename	: GroupTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "GroupTask.h"
#include <utils/SystemLogger.h>

GroupTask::GroupTask()
{
    m_override = false;
    m_algorithm = "basic";

    m_type = "group";
    m_traitName = "groups";
    m_traitValue = "0";
}

bool GroupTask::performTask()
{
	return true;
}

void GroupTask::updateStatus()
{
}

bool GroupTask::readConfigXML(QDomElement taskNode)
{
    LOG("reading group task\n");

    m_name = m_xmlParser.child(taskNode, "Name").attribute("val");
    LOG("name: [%s]\n", m_name.toStdString().c_str());
    m_override = m_xmlParser.child(taskNode, "Override").attribute("val") == "true" ? true : false;
    LOG("override: [%s]\n", (m_override) ? "true" : "false");

    /* files */
    LOG("collecting files to analyze\n");
    QDomElement file = m_xmlParser.child(taskNode, "File");
    while(file.isNull() == false) {
        if(file.attribute("source") == "local") {
            m_taskFiles.push_back(file.attribute("path"));
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
    m_algorithm = m_xmlParser.child(taskNode, "Algorithm").attribute("name");
    LOG("algorithm: [%s]\n", m_algorithm.toStdString().c_str());

    /* output strategies */
    QDomElement out = m_xmlParser.child(taskNode, "Output");
    while(out.isNull() == false) {
        m_exportStrategies.push_back(out.attribute("val"));
        LOG("output strategy: [%s]\n", out.attribute("val").toStdString().c_str());
        out = out.nextSiblingElement("Output");
    }

    return true;
}
