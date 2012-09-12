/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include <debug.h>
#define LOCAL_DEBUG

#include "CoreSystem.h"

CoreSystem::CoreSystem()
{
	loadModules();

	SystemLogger::getInstance()->setStatus(IDLE);
	SystemLogger::getInstance()->setError(NO_ERROR);
	SystemLogger::getInstance()->setLogFile("/home/kuba/analyze_log");
	SystemLogger::getInstance()->setLogLevel(1);

	exploit_counter = 0;
	sample_counter = 0;
}

CoreSystem::~CoreSystem()
{
	/* delete all samples */
	clearSamples();
}

void CoreSystem::addFile(string root_file)
{
	if(root_file.empty())
		return;

	pending_files.push_back(root_file);
}

void CoreSystem::run()
{
	/* parse all input files */
	while(!pending_files.empty()) {
		string current_file = pending_files.front();
		pending_files.pop_front();

		/* check if directory */
		if(isDirectory(current_file)) {
			DIR *dp;
			dirent *de;

			dp = opendir(current_file.c_str());
			if(dp == NULL) {
				PRINTERR("opening directory %s", current_file.c_str());
				continue;
			}

			while((de = readdir(dp))) {
				if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
					continue;

				string next = current_file;
				if(current_file[current_file.size() - 1] != '/')
					next += "/";
				next += de->d_name;
				pending_files.push_front(next);
			}

			continue;
		} /* if */

		/* load */
		if(!load(current_file)) {
			PRINTERR("opening file %s (%s)", current_file.c_str(), getError().c_str());
			continue;
		}

		ShellcodeSample *s;
		while(!samples.empty()) {
			s = samples.front();
			samples.pop_front();

			/* emulate */
			if(!emulate(s)) {
				PRINTERR("emulating %s (%s)", current_file.c_str(), getError().c_str());
				continue;
			}

			/* analyze graph */
			if(!analyze(s)) {
				PRINTERR("analyzing %s (%s)", current_file.c_str(), getError().c_str());
				continue;
			}

			/* make output */
			if(!makeOutput(s)) {
				PRINTERR("output for %s (%s)", current_file.c_str(), getError().c_str());
				continue;
			}

			/* clean up */
			delete s;
		}
	} /* while */
}

void CoreSystem::clear()
{
	exploit_counter = 0;
	sample_counter = 0;

	clearSamples();
}

string CoreSystem::getStatus()
{
	SystemStatus s = SystemLogger::getInstance()->getStatus();
	return mapStatus(s);
}

string CoreSystem::getError()
{
	SystemError e = SystemLogger::getInstance()->getError();
	return mapError(e);
}

void CoreSystem::setLogFile(string filename)
{
	SystemLogger::getInstance()->setLogFile(filename);
}

void CoreSystem::setLogLevel(int level)
{
	SystemLogger::getInstance()->setLogLevel(level);
}

void CoreSystem::setOutput(string method)
{
	output_methods.push_back(method);
}

int CoreSystem::getExploitsNum()
{
	return exploit_counter;
}

int CoreSystem::getSamplesNum()
{
	return sample_counter;
}

string CoreSystem::getVersion()
{
	return VERSION;
}

bool CoreSystem::load(string file)
{
	SystemLogger::getInstance()->setStatus(LOADING);
	SystemLogger::getInstance()->setError(NO_ERROR);

	list<ShellcodeSample *> q;
	ShellcodeSample *s;

	FileAnalyser file_analyser;
	string file_type = file_analyser.analyze(file);

	map<string, AbstractInput *>::iterator it;
	for(it = input_mods->begin(); it != input_mods->end(); ++it) {
		if(file_type == (*it).second->getType()) {
			(*it).second->loadInput(file, &q);

			/* process all returned samples */
			while(!q.empty()) {
				s = q.front();
				q.pop_front();
				samples.push_back(s);
			}

			break;
		}
	}

	SystemLogger::getInstance()->setStatus(IDLE);
	if(!samples.empty()) {
		++sample_counter;
		return true;
	}

	SystemLogger::getInstance()->setError(CANNOT_HANDLE_FILE);
	return false;
}

bool CoreSystem::emulate(ShellcodeSample *s)
{
	SystemLogger::getInstance()->setStatus(EMULATING);
	SystemLogger::getInstance()->setError(NO_ERROR);

	emu_system.loadSample(s);
	bool ret = emu_system.emulate();

	SystemLogger::getInstance()->setStatus(IDLE);
	if(!ret) {
		SystemLogger::getInstance()->setError(EMULATION_FAILED);
		return false;
	}

	return true;
}

bool CoreSystem::analyze(ShellcodeSample *s)
{
	SystemLogger::getInstance()->setStatus(ANALYZING);
	SystemLogger::getInstance()->setError(NO_ERROR);

	ana_system.loadSample(s);
	bool ret = ana_system.analyze();

	SystemLogger::getInstance()->setStatus(IDLE);
	if(!ret) {
		SystemLogger::getInstance()->setError(ANALYZING_FAILED);
		return false;
	}

	if(s->getInfo()->isShellcodePresent())
		++exploit_counter;
	return true;
}

bool CoreSystem::makeOutput(ShellcodeSample *s)
{
	SystemLogger::getInstance()->setStatus(ANALYZING);
	SystemLogger::getInstance()->setError(NO_ERROR);

	list<string>::iterator it;
	if(!s->getInfo()->isShellcodePresent())
		return true;

	bool ret;
	for(it = output_methods.begin(); it != output_methods.end(); ++it) {
		ret = (*output_mods)[*it]->generateOutput(s);

		SystemLogger::getInstance()->setStatus(IDLE);
		if(!ret) {
			SystemLogger::getInstance()->setError(OUTPUT_FAILED);
			return false;
		}
	}

	return true;
}

string CoreSystem::mapStatus(SystemStatus status)
{
	return SystemLogger::getInstance()->mapStatus(status);
}

string CoreSystem::mapError(SystemError error)
{
	return SystemLogger::getInstance()->mapError(error);
}

void CoreSystem::loadModules()
{
	input_mods = ModuleManager::getInstance()->getInput();
	output_mods = ModuleManager::getInstance()->getOutput();
}

void CoreSystem::clearSamples()
{
	list<ShellcodeSample *>::iterator it;
	for(it = samples.begin(); it != samples.end(); ++it) {
		delete &(*it);
		samples.erase(it);
	}
}
