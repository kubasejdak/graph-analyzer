/*
 *  Filename	: DescriptionExportStrategy.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 17-04-2013
 */

#include "DescriptionExportStrategy.h"

#include <string>

#include <utils/DatabaseManager.h>
#include <utils/SystemLogger.h>
#include <core/version.h>
#include <tasks/ITask.h>

using namespace std;

void XMLDescriptionExportStrategy::exportDescription()
{
    if(!m_xmlParser.open(STATUS_FILE)) {
        LOG_ERROR("FAILURE\n\n");
        return;
    }

    if(m_xmlParser.hasRoot("SystemStatus")) {
        QDomElement s = m_xmlParser.root("SystemStatus");
        m_xmlParser.removeRoot(s);
    }

    QDomElement statusNode = m_xmlParser.createRoot("SystemStatus");

    m_xmlParser.createChild(statusNode, "Version").setAttribute("val", VERSION);
	m_xmlParser.createChild(statusNode, "CurrentTask").setAttribute("name", SystemLogger::instance()->status().c_str());
	m_xmlParser.createChild(statusNode, "LastError").setAttribute("desc", SystemLogger::instance()->error().c_str());
    m_xmlParser.createChild(statusNode, "ErrorsNum").setAttribute("val", SystemLogger::instance()->errorsNum());;

    m_xmlParser.close();
}

string XMLDescriptionExportStrategy::description()
{
    return "xml";
}
