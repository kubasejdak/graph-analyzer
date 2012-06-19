/*
 *  Filename	: AnalysisSystem.h
 *  Author	: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef ANALYSISSYSTEM_H_
#define ANALYSISSYSTEM_H_

/* standard headers */
#include <map>
#include <string>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;

/* project headers */
#include <core/EmulationSystem.h>
#include <core/ShellcodeSample.h>
#include <core/ShellcodeInfo.h>
#include <modules/AbstractInput.h>
#include <modules/AbstractOutput.h>
#include <modules/ModuleManager.h>

enum SystemStatus {
	IDLE,
	ERROR,
	WORKING
};

class AnalysisSystem {
public:
	AnalysisSystem();
	virtual ~AnalysisSystem();

	bool loadShellcode(string filename);
	bool analyze(string filename);
	ShellcodeInfo getResults(string filename);
	bool generateOutput(string filename, int method, string *output);
	SystemStatus getStatus();
	void clearCache();

private:
	EmulationSystem emuSystem;
	map<int, AbstractInput *> *inputModules;
	map<int, AbstractOutput *> *outputModules;
	map<string, ShellcodeSample *> samples;
	SystemStatus status;

	void loadModules();
	void changeStatus(SystemStatus status);
};

#endif /* ANALYSISSYSTEM_H_ */
