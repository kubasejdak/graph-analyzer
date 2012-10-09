/*
 * Filename		: ConfigFile.cpp
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::read()
{
	opt::options_description config("config");
	config.add_options()
		("APP_ROOT_PATH", opt::value<string>(), "path to root folder of GraphAnalyzer resources")
		("EMULATION_STEPS", opt::value<int>(), "number of emulation steps for libemu")
		("GRAPHS_DIR", opt::value<string>(), "directory to store generated graphs")
		("DELETE_CODE_INSTANTLY", opt::value<bool>(), "should code be deleted right after analysis")
		("DB_QT_DRIVER", opt::value<string>(), "Qt driver for database")
		("DB_HOST", opt::value<string>(), "database host")
		("DB_NAME", opt::value<string>(), "database name")
		("DB_USER", opt::value<string>(), "database user")
		("DB_PASS", opt::value<string>(), "database password")
		("ANA_TABLE_PREFIX", opt::value<string>(), "prefix for all analyze tables in database")
		("OPT_TABLE_PREFIX", opt::value<string>(), "prefix for all options tables in database")
		("ID_SEQ_POSTFIX", opt::value<string>(), "postfix for all GraphAnalyzer sequences in database")
		("SKIP_NONEXPLOIT_OUTPUT", opt::value<bool>(), "should samples without exploits be skipped");
	opt::variables_map vm;

	opt::store(opt::parse_config_file<char>(CONFIG_FILE, config), vm);
	opt::notify(vm);

	if(vm.count("APP_ROOT_PATH")) {
		APP_ROOT_PATH = vm["APP_ROOT_PATH"].as<string>();
		APP_ROOT_PATH = addSlash(APP_ROOT_PATH);
	}
	else {
		LOG_ERROR("APP_ROOT_PATH not set in config file.\n");
		exit(1);
	}
	if(vm.count("EMULATION_STEPS"))
		EMULATION_STEPS = vm["EMULATION_STEPS"].as<int>();
	else {
		LOG_ERROR("EMULATION_STEPS not set in config file.\n");
		exit(1);
	}
	if(vm.count("GRAPHS_DIR")) {
		GRAPHS_DIR = vm["GRAPHS_DIR"].as<string>();
		GRAPHS_DIR = addSlash(GRAPHS_DIR);
		if(isRelative(GRAPHS_DIR))
			GRAPHS_DIR.insert(0, APP_ROOT_PATH);
	}
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
	if(vm.count("ANA_TABLE_PREFIX"))
		ANA_TABLE_PREFIX = vm["ANA_TABLE_PREFIX"].as<string>();
	else {
		LOG_ERROR("ANA_TABLE_PREFIX not set in config file.\n");
		exit(1);
	}
	if(vm.count("OPT_TABLE_PREFIX"))
		OPT_TABLE_PREFIX = vm["OPT_TABLE_PREFIX"].as<string>();
	else {
		LOG_ERROR("OPT_TABLE_PREFIX not set in config file.\n");
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
