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

#include <core/EmulationSystem.h>
#include <core/AnalysisSystem.h>
#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/FileTypeAnalyzer.h>
#include <utils/SystemLogger.h>
#include <core/version.h>
#include <utils/Toolbox.h>
#include <grouping/GroupManager.h>
#include <modules/input/AbstractInput.h>
#include <modules/output/AbstractOutput.h>
#include <modules/ModuleManager.h>

class CoreSystem {
public:
	CoreSystem();
	virtual ~CoreSystem();

	/* basic sample operation */
    int addFile(QString file);
    int run();
	void remakeGroups();
	void clear();

	/* status and error */
    QString status();
    QString error();
	int progress();

	/* logging */
    void setLogFile(QString file);
	void setLogLevel(int level);

	/* results */
	int filesNum();
    int exploitsNum();
    int samplesNum();
	int errorsNum();

	/* utility */
	void setOutput(QString method);
    QString version();
	bool readOptions();
	bool dbUpdateSystemInfo();
	bool dbRemovePendingFiles();

private:
	/* function members */
	int load(QString file);
	bool emulate(ExploitSample *s);
	bool analyze(ExploitSample *s);
	bool makeOutput(ExploitSample *s);
	void processGroup(ExploitSample *s);

	void loadModules();
	void clearSamples();

	/* data members */
    EmulationSystem m_emuSystem;
    AnalysisSystem m_anaSystem;
	InputMap *m_inputMods;
	OutputMap *m_outputMods;

    QList<ExploitSample *> m_samples;
    QList<QString> m_pendingFiles;
    QList<QString> m_outputMethods;

	int m_fileCounter;			/* number of files extracted to analyze */
	int m_processedCounter;		/* number of processed files */
	int m_sampleCounter;		/* number of files that were loaded */
	int m_exploitCounter;		/* number of detected exploits */
	int m_errorCounter;			/* number od errors */

	GroupManager m_groupManager;
};

#endif /* CORESYSTEM_H_ */
