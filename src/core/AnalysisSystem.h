/*
 * Filename		: AnalysisSystem.h
 * Author		: Kuba Sejdak
 * Created on	: 21-06-2012
 */

#ifndef ANALYSISSYSTEM_H_
#define ANALYSISSYSTEM_H_

#include <QMap>

#include <core/ExploitSample.h>
#include <modules/analyze/AbstractAnalyze.h>
#include <modules/ModuleManager.h>

class AnalysisSystem {
public:
	AnalysisSystem();
	virtual ~AnalysisSystem();

	void loadSample(ExploitSample *sample);
	bool analyze();

private:
	void loadModules();

    ExploitSample *m_sample;
	AnalyzeMap *m_analyzeModules;
};

#endif /* ANALYSISSYSTEM_H_ */
