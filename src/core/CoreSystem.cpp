/*
 *  Filename	: CoreSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "CoreSystem.h"

CoreSystem::CoreSystem()
{
	loadModules();

	SystemLogger::getInstance()->setStatus(IDLE);
	SystemLogger::getInstance()->setError(NO_ERROR);
	SystemLogger::getInstance()->setLogFile("/home/kuba/analyzer_log");
	SystemLogger::getInstance()->setLogLevel(1);

	exploit_counter = 0;
	sample_counter = 0;

	readConfig();
	LOG("created CoreSystem instance\n");
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
				LOG_ERROR("opening directory %s\n", current_file.c_str());
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
			LOG_ERROR("opening file %s (%s)\n", current_file.c_str(), getError().c_str());
			continue;
		}

		ShellcodeSample *s;
		while(!samples.empty()) {
			s = samples.front();
			samples.pop_front();

			/* emulate */
			if(!emulate(s)) {
				LOG_ERROR("emulating %s (%s)\n", current_file.c_str(), getError().c_str());
				continue;
			}

			/* analyze graph */
			if(!analyze(s)) {
				LOG_ERROR("analyzing %s (%s)\n", current_file.c_str(), getError().c_str());
				continue;
			}

			/* make output */
			if(!makeOutput(s)) {
				LOG_ERROR("output for %s (%s)\n", current_file.c_str(), getError().c_str());
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

void CoreSystem::readConfig()
{
	opt::options_description config("config");
	config.add_options()
		("EMULATION_STEPS", opt::value<int>(), "number of emulation steps for libemu")
		("GRAPHS_DIR", opt::value<string>(), "directory to store generated graphs")
		("DELETE_CODE_INSTANTLY", opt::value<bool>(), "should code be deleted right after analysis")
		("DB_QT_DRIVER", opt::value<string>(), "Qt driver for database")
		("DB_HOST", opt::value<string>(), "database host")
		("DB_NAME", opt::value<string>(), "database name")
		("DB_USER", opt::value<string>(), "database user")
		("DB_PASS", opt::value<string>(), "database password")
		("TABLE_PREFIX", opt::value<string>(), "prefix for all GraphAnalyzer tables in database")
		("ID_SEQ_POSTFIX", opt::value<string>(), "postfix for all GraphAnalyzer sequences in database")
		("SKIP_NONEXPLOIT_OUTPUT", opt::value<bool>(), "should samples without exploits be skipped")
	;
	opt::variables_map vm;

	opt::store(opt::parse_config_file<char>("../conf/analyzer.conf", config), vm);
	opt::notify(vm);

	if(vm.count("EMULATION_STEPS"))
		EMULATION_STEPS = vm["EMULATION_STEPS"].as<int>();
	else {
		LOG_ERROR("EMULATION_STEPS not set in config file.\n");
		exit(1);
	}
	if(vm.count("GRAPHS_DIR"))
		GRAPHS_DIR = vm["GRAPHS_DIR"].as<string>();
	else {
		LOG_ERROR("GRAPHS_DIR not set in config file.\n");
		exit(1);
	}
	if(vm.count("DELETE_CODE_INSTANTLY"))
		DELETE_CODE_INSTANTLY = vm["DELETE_CODE_INSTANTLY"].as<bool>();
	else {
		LOG_ERROR("DELETE_CODE_INSTANTLY not set in config file.\n");
		exit(1);
	}
	if(vm.count("DB_QT_DRIVER"))
		DB_QT_DRIVER = vm["DB_QT_DRIVER"].as<string>();
	else {
		LOG_ERROR("DB_QT_DRIVER not set in config file.\n");
		exit(1);
	}
	if(vm.count("DB_HOST"))
		DB_HOST = vm["DB_HOST"].as<string>();
	else {
		LOG_ERROR("DB_HOST not set in config file.\n");
		exit(1);
	}
	if(vm.count("DB_NAME"))
		DB_NAME = vm["DB_NAME"].as<string>();
	else {
		LOG_ERROR("DB_NAME not set in config file.\n");
		exit(1);
	}
	if(vm.count("DB_USER"))
		DB_USER = vm["DB_USER"].as<string>();
	else {
		LOG_ERROR("DB_USER not set in config file.\n");
		exit(1);
	}
	if(vm.count("DB_PASS"))
		DB_PASS = vm["DB_PASS"].as<string>();
	else {
		LOG_ERROR("DB_PASS not set in config file.\n");
		exit(1);
	}
	if(vm.count("TABLE_PREFIX"))
		TABLE_PREFIX = vm["TABLE_PREFIX"].as<string>();
	else {
		LOG_ERROR("TABLE_PREFIX not set in config file.\n");
		exit(1);
	}
	if(vm.count("ID_SEQ_POSTFIX"))
		ID_SEQ_POSTFIX = vm["ID_SEQ_POSTFIX"].as<string>();
	else {
		LOG_ERROR("ID_SEQ_POSTFIX not set in config file.\n");
		exit(1);
	}
	if(vm.count("SKIP_NONEXPLOIT_OUTPUT"))
		SKIP_NONEXPLOIT_OUTPUT = vm["SKIP_NONEXPLOIT_OUTPUT"].as<bool>();
	else {
		LOG_ERROR("SKIP_NONEXPLOIT_OUTPUT not set in config file.\n");
		exit(1);
	}
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
