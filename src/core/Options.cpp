/*
 * Filename		: Options.cpp
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#include "Options.h"

#include <string>
#include <QDomElement>

#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>

using namespace std;

Options::Options()
{
	// initial configuration
	graphsDir = "/var/www/jsejdak/graphs";
    tmpPcapDir = "/tmp/tmp_pcap";
	emulationSteps = 1000000;
	skipBigFiles = true;
	bigFileSize = 20971520;
	skipBrokenSamples = true;
	brokenSampleSize = 50;

	if(!readConfigXML())
		LOG_ERROR("failed to read XML configuration, using default options\n");

	listOptions();
	LOG("SUCCESS\n\n");
}

bool Options::readConfigXML()
{
	if(!m_xmlParser.open(CONFIG_FILE)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	if(!m_xmlParser.hasRoot("CoreOptions")) {
		m_xmlParser.close();
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	QDomElement options = m_xmlParser.root("CoreOptions");

    graphsDir = m_xmlParser.child(options, "GraphsDir").attribute("path").toStdString();

    tmpPcapDir = m_xmlParser.child(options, "TmpPcapDir").attribute("path").toStdString();

    tcpflowParams = m_xmlParser.child(options, "TcpflowParams").attribute("val").toStdString();

	emulationSteps = m_xmlParser.child(options, "EmulationSteps").attribute("val").toInt();

	skipBigFiles = m_xmlParser.child(options, "SkipBigFiles").attribute("val") == "true" ? true : false;
	bigFileSize = m_xmlParser.child(options, "SkipBigFiles").attribute("size").toInt();

	skipBrokenSamples = m_xmlParser.child(options, "SkipBrokenSamples").attribute("val") == "true" ? true : false;
	brokenSampleSize = m_xmlParser.child(options, "SkipBrokenSamples").attribute("size").toInt();

	m_xmlParser.close();
	LOG("SUCCESS\n\n");
	return true;
}

void Options::listOptions()
{
	// core settings
	LOG("core settings:\n");
	LOG("graphsDir: [%s]\n", graphsDir.c_str());
	LOG("tmpPcapDir: [%s]\n", tmpPcapDir.c_str());
	LOG("tcpflowParams: [%s]\n", tcpflowParams.c_str());
	LOG("emulationSteps: [%d]\n", emulationSteps);
	LOG("skipBigFiles: [%s]\n", skipBigFiles ? "true" : "false");
	LOG("bigFileSize: [%d]\n", bigFileSize);
	LOG("skipBrokenSamples: [%s]\n", skipBrokenSamples ? "true" : "false");
	LOG("brokenSampleSize: [%d]\n", brokenSampleSize);
	LOG("SUCCESS\n\n");
}
