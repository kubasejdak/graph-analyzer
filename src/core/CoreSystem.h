/*
 *  Filename	: CoreSystem.h
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef CORESYSTEM_H_
#define CORESYSTEM_H_

/* standard headers */
#include <map>
#include <string>
#include <queue>
using namespace std;

/* project headers */
#include <core/EmulationSystem.h>
#include <core/AnalysisSystem.h>
#include <core/ShellcodeSample.h>
#include <core/ShellcodeInfo.h>
#include <core/FileAnalyser.h>
#include <core/SystemLogger.h>
#include <modules/input/AbstractInput.h>
#include <modules/output/AbstractOutput.h>
#include <modules/ModuleManager.h>

class CoreSystem {
public:
	CoreSystem();
	virtual ~CoreSystem();

	list<string> load(string filename);
	void emulate(string filename);
	void analyze(string filename);
	ShellcodeInfo *getResults(string filename);
	bool generateOutput(string filename, int method, string *output);
	SystemStatus getStatus();
	SystemError getError();
	string mapError(SystemError error);
	void clearCache();

private:
	EmulationSystem emuSystem;
	AnalysisSystem anaSystem;
	map<int, AbstractInput *> *inputModules;
	map<int, AbstractOutput *> *outputModules;
	map<string, ShellcodeSample *> samples;

	void loadModules();
};

#endif /* CORESYSTEM_H_ */
