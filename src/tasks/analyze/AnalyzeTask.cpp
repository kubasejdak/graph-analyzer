/*
 *  Filename	: AnalyzeTask.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#include "AnalyzeTask.h"

bool AnalyzeTask::perform()
{
	return true;
}

bool AnalyzeTask::readConfigXML()
{
	if(!m_xmlParser.open(TASKS_FILE))
		return false;

	if(!m_xmlParser.hasRoot("Task")) {
		m_xmlParser.close();
		return false;
	}

	QDomElement options = m_xmlParser.root("Task");

	graphsDir = m_xmlParser.child(options, "GraphsDir").attribute("path");

	emulationSteps = m_xmlParser.child(options, "EmulationSteps").attribute("val").toInt();

	skipBigFiles = m_xmlParser.child(options, "SkipBigFiles").attribute("val") == "true" ? true : false;
	bigFileSize = m_xmlParser.child(options, "SkipBigFiles").attribute("size").toInt();

	skipBrokenSamples = m_xmlParser.child(options, "SkipBrokenSamples").attribute("val") == "true" ? true : false;
	brokenSampleSize = m_xmlParser.child(options, "SkipBrokenSamples").attribute("size").toInt();

	skipEmptySamples = m_xmlParser.child(options, "SkipEmptySamples").attribute("val") == "true" ? true : false;

	resemblenceLevel = m_xmlParser.child(options, "ResemblenceLevel").attribute("val").toInt();

	m_xmlParser.close();
	return true;
}
