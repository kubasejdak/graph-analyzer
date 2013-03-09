/*
 *  Filename	: CoreSystem.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef CORESYSTEM_H_
#define CORESYSTEM_H_

#include <QMap>
#include <QMapIterator>
#include <QList>
#include <QString>
#include <QDir>
#include <QFile>

#include <core/RPCTagResolver.h>
#include <core/EmulationSystem.h>
#include <core/AnalysisSystem.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/FileTypeAnalyzer.h>
#include <utils/SystemLogger.h>
#include <utils/XMLParser.h>
#include <core/version.h>
#include <utils/Toolbox.h>
#include <grouping/GroupManager.h>
#include <modules/input/AbstractInput.h>
#include <modules/output/AbstractOutput.h>
#include <modules/ModuleManager.h>

#define TASKS_FILE		"/var/www/jsejdak/GraphAnalyzerTasks.xml"

class CoreSystem {
public:
	CoreSystem();
	virtual ~CoreSystem();

	/* system interface */
	bool execute(QString funcName);

	/* status and error */
	QString status();
	QString lastError();
	QString version();

//--------------------------------------------------------------
	//bool dbUpdateSystemInfo();
//--------------------------------------------------------------

private:
	/* RPC functions */
	bool rpcAnalyze();
	bool rpcExperiment();
	bool rpcExportVersion();

	/* core operations */
	int load(QString filename);
	bool emulate(ExploitSample *s);
	bool analyze(ExploitSample *s);
	bool exportResults(ExploitSample *s);

	/* utility */
	bool readAnalyzeTaskXML();
	void listScheduledFiles();
	int addScheduledFiles();
	int addFile(QString filename);
	void loadModules();
	void clearSamples();

	/* data members */
	RPCTagResolver m_tagResolver;
	QList<QString> m_scheduledFiles;
	QList<QString> m_taskFiles;

    EmulationSystem m_emuSystem;
    AnalysisSystem m_anaSystem;
	InputMap *m_inputMods;
	OutputMap *m_outputMods;
	SampleContainer m_samples;
    QList<QString> m_outputMethods;
	GroupManager m_groupManager;
	XMLParser m_xmlParser;
};

#endif /* CORESYSTEM_H_ */
