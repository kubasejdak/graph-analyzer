/*
 * Filename		: AnalysisSystem.h
 * Author		: Kuba Sejdak
 * Created on	: 21-06-2012
 */

#ifndef ANALYSISSYSTEM_H_
#define ANALYSISSYSTEM_H_

class QMap;

class ExploitSample;
class IAnalyze;
class ModuleManager;

class AnalysisSystem {
public:
	AnalysisSystem();
	virtual ~AnalysisSystem() {}

	void loadSample(ExploitSample *sample);
	bool analyze();

private:
	void loadModules();

    ExploitSample *m_sample;
	AnalyzeMap *m_analyzeMods;
};

#endif /* ANALYSISSYSTEM_H_ */
