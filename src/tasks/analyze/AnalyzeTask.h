/*
 *  Filename	: AnalyzeTask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#ifndef ANALYZETASK_H
#define ANALYZETASK_H

class QList;
class QString;

class ITask;
class AnalysisSystem;
class EmulationSystem;
class ModuleManager;
class ExploitSample;

class AnalyzeTask : public ITask {
public:
	AnalyzeTask();
	virtual ~AnalyzeTask() {}

private:
	void loadModules();
    virtual bool performTask();
	virtual void updateStatus();
	virtual bool readConfigXML(QDomElement taskNode);
	void addScheduledFile(QString filename);

	int load(QString filename);
	bool emulate(ExploitSample *s);
	bool analyze(ExploitSample *s);
	bool exportResults(ExploitSample *s);

	EmulationSystem m_emulationSystem;
	AnalysisSystem m_analysisSystem;

	/* task specific info */
    QList<QString> m_taskFiles;
	bool m_override;
	int m_scheduledFiles;
	int m_loadedFiles;
	int m_analyzedSamples;
	int m_detectedExploits;
	int m_allTaskFiles;

	InputMap *m_inputMods;
	AnalyzeMap *m_analyzeMods;
	OutputMap *m_outputMods;
};

#endif /* ANALYZETASK_H */
