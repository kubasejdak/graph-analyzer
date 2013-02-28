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
	bool stat = false;
	if(!init(CONFIG_FILE))
		return stat;

	/* parse root nodes */
	for(int i = 0; i < m_roots.size(); ++i) {
		QDomElement rootNode = m_roots.at(i).toElement();
		/* core options */
		if(rootNode.tagName() == "CoreOptions") {
			QDomNodeList options = rootNode.childNodes();
			stat = true;

			for(int i = 0; i < options.count(); ++i) {
				QDomElement e = options.at(i).toElement();
				QString node = e.tagName();

				if(node == "Logging") {
					logLevel = e.attribute("level").toInt();
					logFile = e.attribute("file");
				}
				else if(node == "GraphsDir") {
					graphsDir = e.attribute("path");
				}
				else if(node == "EmulationSteps") {
					emulationSteps = e.attribute("val").toInt();
				}
				else if(node == "SkipBigFiles") {
					skipBigFiles = (e.attribute("val") == "true") ? true : false;
					bigFileSize = e.attribute("size").toInt();
				}
				else if(node == "SkipBrokenSamples") {
					skipBrokenSamples = (e.attribute("val") == "true") ? true : false;
					brokenSampleSize = e.attribute("size").toInt();
				}
				else if(node == "SkipEmptySamples") {
					skipEmptySamples = (e.attribute("val") == "true") ? true : false;
				}
				else if(node == "ResemblenceLevel") {
					resemblenceLevel = e.attribute("level").toInt();
				}
			}
		} /* CoreOptions */

		/* pending files */
		if(rootNode.tagName() == "PendingFiles") {
			QDomNodeList options = rootNode.childNodes();

			for(int i = 0; i < options.count(); ++i) {
				QDomElement e = options.at(i).toElement();
				pendingFiles.push_back(e.text());
			}
		} /* PendingFiles */
	}

	return stat;
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

	/* pending files */
	LOG("pending files (from database):\n");
	for(int i = 0; i < pendingFiles.size(); ++i)
		LOG("file: [%s]\n", pendingFiles.at(i).toStdString().c_str());
}
