/*
 *  Filename	: ExportTask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 21-07-2013
 */

#ifndef EXPORTTASK_H
#define EXPROTTASK_H

#include <string>
#include <list>
#include <QDomElement>

#include <tasks/ITask.h>
#include <tasks/export/modules/ModulesManager.h>
#include <tasks/analyze/modules/ModulesManager.h>
#include <tasks/group/modules/ModulesManager.h>

class ExploitSample;

class ExportTask : public ITask {
public:
	ExportTask();
	virtual ~ExportTask() {}

private:
	void loadModules();
    virtual bool performTask();
	virtual void updateStatus();
	virtual bool readConfigXML(QDomElement taskNode);

	int load();
	bool loadCode();
	bool exportResults(ExploitSampleHandle s);

	ExploitSampleHandle findSample(std::string name, std::string extractedFrom);

	// task specific info
	std::string m_exportDir;
	int m_exportedSamples;
	int m_scheduledSamples;
	std::string m_inputStrategy;

	Group::InputMap *m_inputMods;
	Analyze::InputMap *m_codeInputMods;
	Export::OutputMap *m_outputMods;
};

#endif /* EXPORTTASK_H */
