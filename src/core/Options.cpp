/*
 * Filename		: Options.cpp
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#include <core/Options.h>
#include <utils/DatabaseManager.h>

Options::Options()
{
	/* initial configuration */
	logLevel = 2;
	logFile = "/var/www/jsejdak/GraphAnalyzer.log";
	graphsDir = "/var/www/jsejdak/graphs";
	emulationSteps = 1000000;
	skipBigFiles = true;
	bigFileSize = 20971520;
	skipBrokenSamples = true;
	brokenSampleSize = 50;
	skipEmptySamples = true;
	resemblenceLevel = 50;
}

bool Options::readConfigXML()
{
	if(!m_xmlParser.open(CONFIG_FILE))
		return false;

	if(!m_xmlParser.hasRoot("CoreOptions")) {
		m_xmlParser.close();
		return false;
	}

	QDomElement options = m_xmlParser.root("CoreOptions");

	logLevel = m_xmlParser.child(options, "Logging").attribute("level").toInt();
	logFile = m_xmlParser.child(options, "Logging").attribute("file");

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

bool Options::readPendingFilesXML()
{
	if(!m_xmlParser.open(CONFIG_FILE))
		return false;

	if(!m_xmlParser.hasRoot("PendingFiles")) {
		m_xmlParser.close();
		return false;
	}

	QDomElement files = m_xmlParser.root("PendingFiles");

	while(m_xmlParser.hasChild(files, "File")) {
		QDomElement f = m_xmlParser.child(files, "File");
		pendingFiles.push_back(f.text());
		m_xmlParser.removeChild(files, f);
	}
	m_xmlParser.removeRoot(files);

	m_xmlParser.close();
	return true;
}

void Options::listOptions()
{
	/* core settings */
	LOG("core settings:\n");
	LOG("logLevel: [%d]\n", logLevel);
	LOG("logFile: [%s]\n", logFile.toStdString().c_str());
	LOG("graphsDir: [%s]\n", graphsDir.toStdString().c_str());
	LOG("emulationSteps: [%d]\n", emulationSteps);
	LOG("skipBigFiles: [%s]\n", skipBigFiles ? "true" : "false");
	LOG("bigFileSize: [%d]\n", bigFileSize);
	LOG("skipBrokenSamples: [%s]\n", skipBrokenSamples ? "true" : "false");
	LOG("brokenSampleSize: [%d]\n", brokenSampleSize);
	LOG("skipEmptySamples: [%s]\n", skipEmptySamples ? "true" : "false");
	LOG("resemblenceLevel: [%d]\n", resemblenceLevel);
}

void Options::listPendingFiles()
{
	/* pending files */
	LOG("pending files:\n");
	for(int i = 0; i < pendingFiles.size(); ++i)
		LOG("file: [%s]\n", pendingFiles.at(i).toStdString().c_str());
}
