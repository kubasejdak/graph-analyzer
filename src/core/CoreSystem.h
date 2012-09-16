/*
 *  Filename	: CoreSystem.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef CORESYSTEM_H_
#define CORESYSTEM_H_

/* standard headers */
#include <map>
#include <list>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

/* project headers */
#include <core/ConfigFile.h>
#include <core/EmulationSystem.h>
#include <core/AnalysisSystem.h>
#include <core/ShellcodeSample.h>
#include <core/ShellcodeInfo.h>
#include <core/FileAnalyser.h>
#include <core/SystemLogger.h>
#include <core/version.h>
#include <modules/input/AbstractInput.h>
#include <modules/output/AbstractOutput.h>
#include <modules/ModuleManager.h>
#include <toolbox.h>

class CoreSystem {
public:
	CoreSystem();
	virtual ~CoreSystem();

	/* basic sample operation */
	void addFile(string root_file);
	void run();
	void clear();

	/* status and error */
	string getStatus();
	string getError();

	/* logging */
	void setLogFile(string filename);
	void setLogLevel(int level);

	/* results */
	void setOutput(string method);
	int getExploitsNum();
	int getSamplesNum();

	/* utility */
	string getVersion();
	void readConfig();

private:
	/* function members */
	bool load(string file);
	bool emulate(ShellcodeSample *s);
	bool analyze(ShellcodeSample *s);
	bool makeOutput(ShellcodeSample *s);

	string mapStatus(SystemStatus status);
	string mapError(SystemError error);

	void loadModules();
	void clearSamples();

	/* data members */
	EmulationSystem emu_system;
	AnalysisSystem ana_system;
	map<string, AbstractInput *> *input_mods;
	map<string, AbstractOutput *> *output_mods;

	list<ShellcodeSample *> samples;
	list<string> pending_files;
	list<string> output_methods;

	int exploit_counter;
	int sample_counter;
};

#endif /* CORESYSTEM_H_ */
