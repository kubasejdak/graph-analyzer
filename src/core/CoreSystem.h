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
#include <core/ShellcodeSample.h>
#include <core/ShellcodeInfo.h>
#include <core/FileAnalyser.h>
#include <core/SystemLogger.h>
#include <core/version.h>
#include <core/Toolbox.h>
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
	void makeGroups();
	void clear();

	/* status and error */
    QString status();
    QString error();

	/* logging */
    void setLogFile(QString file);
	void setLogLevel(int level);

	/* results */
    void setOutput(QString method);
    int exploitsNum();
    int samplesNum();

	/* utility */
    QString version();
	bool readOptions();

private:
	/* function members */
    bool load(QString file);
	bool emulate(ShellcodeSample *s);
	bool analyze(ShellcodeSample *s);
	bool makeOutput(ShellcodeSample *s);

	void loadModules();
	void clearSamples();

	/* data members */
    EmulationSystem m_emuSystem;
    AnalysisSystem m_anaSystem;
    QMap<QString, AbstractInput *> *m_inputMods;
    QMap<QString, AbstractOutput *> *m_outputMods;

    QList<ShellcodeSample *> m_samples;
    QList<QString> m_pendingFiles;
    QList<QString> m_outputMethods;

    int m_exploitCounter;
    int m_sampleCounter;
};

#endif /* CORESYSTEM_H_ */
