/*
 * Filename		: ConfigFile.cpp
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#include "ConfigFile.h"

void ConfigFile::read()
{
    LOG("\n");
    LOG("reading config file\n");
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
        APP_ROOT_PATH = vm["APP_ROOT_PATH"].as<string>().c_str();
        APP_ROOT_PATH = Toolbox::removeSlash(APP_ROOT_PATH);
        APP_ROOT_PATH = Toolbox::makeRelative(APP_ROOT_PATH);
        LOG("APP_ROOT_PATH: [%s]\n", APP_ROOT_PATH.toStdString().c_str());
	}
	else {
        LOG_ERROR("APP_ROOT_PATH not set in config file\n");
		exit(1);
	}
    if(vm.count("EMULATION_STEPS")) {
		EMULATION_STEPS = vm["EMULATION_STEPS"].as<int>();
        LOG("EMULATION_STEPS: [%d]\n", EMULATION_STEPS);
    }
	else {
        LOG_ERROR("EMULATION_STEPS not set in config file\n");
		exit(1);
	}
	if(vm.count("GRAPHS_DIR")) {
        GRAPHS_DIR = vm["GRAPHS_DIR"].as<string>().c_str();
        GRAPHS_DIR = Toolbox::removeSlash(GRAPHS_DIR);
        GRAPHS_DIR = Toolbox::makeRelative(GRAPHS_DIR);
        LOG("GRAPHS_DIR: [%s]\n", GRAPHS_DIR.toStdString().c_str());
	}
	else {
        LOG_ERROR("GRAPHS_DIR not set in config file\n");
		exit(1);
	}
    if(vm.count("DELETE_CODE_INSTANTLY")) {
		DELETE_CODE_INSTANTLY = vm["DELETE_CODE_INSTANTLY"].as<bool>();
        LOG("DELETE_CODE_INSTANTLY: [%s]\n", (DELETE_CODE_INSTANTLY) ? "true" : "false");
    }
	else {
        LOG_ERROR("DELETE_CODE_INSTANTLY not set in config file\n");
		exit(1);
	}
    if(vm.count("DB_QT_DRIVER")) {
        DB_QT_DRIVER = vm["DB_QT_DRIVER"].as<string>().c_str();
        LOG("DB_QT_DRIVER: [%s]\n", DB_QT_DRIVER.toStdString().c_str());
    }
	else {
        LOG_ERROR("DB_QT_DRIVER not set in config file\n");
		exit(1);
	}
    if(vm.count("DB_HOST")) {
        DB_HOST = vm["DB_HOST"].as<string>().c_str();
        LOG("DB_HOST: [%s]\n", DB_HOST.toStdString().c_str());
    }
	else {
        LOG_ERROR("DB_HOST not set in config file\n");
		exit(1);
	}
    if(vm.count("DB_NAME")) {
        DB_NAME = vm["DB_NAME"].as<string>().c_str();
        LOG("DB_NAME: [%s]\n", DB_NAME.toStdString().c_str());
    }
	else {
        LOG_ERROR("DB_NAME not set in config file\n");
		exit(1);
	}
    if(vm.count("DB_USER")) {
        DB_USER = vm["DB_USER"].as<string>().c_str();
        LOG("DB_USER: [%s]\n", DB_USER.toStdString().c_str());
    }
	else {
        LOG_ERROR("DB_USER not set in config file\n");
		exit(1);
	}
    if(vm.count("DB_PASS")) {
        DB_PASS = vm["DB_PASS"].as<string>().c_str();
        LOG("DB_PASS: [%s]\n", DB_PASS.toStdString().c_str());
    }
	else {
        LOG_ERROR("DB_PASS not set in config file\n");
		exit(1);
	}
    if(vm.count("ANA_TABLE_PREFIX")) {
        ANA_TABLE_PREFIX = vm["ANA_TABLE_PREFIX"].as<string>().c_str();
        LOG("ANA_TABLE_PREFIX: [%s]\n", ANA_TABLE_PREFIX.toStdString().c_str());
    }
	else {
        LOG_ERROR("ANA_TABLE_PREFIX not set in config file\n");
		exit(1);
	}
    if(vm.count("OPT_TABLE_PREFIX")) {
        OPT_TABLE_PREFIX = vm["OPT_TABLE_PREFIX"].as<string>().c_str();
        LOG("OPT_TABLE_PREFIX: [%s]\n", OPT_TABLE_PREFIX.toStdString().c_str());
    }
	else {
        LOG_ERROR("OPT_TABLE_PREFIX not set in config file\n");
		exit(1);
	}
    if(vm.count("ID_SEQ_POSTFIX")) {
        ID_SEQ_POSTFIX = vm["ID_SEQ_POSTFIX"].as<string>().c_str();
        LOG("ID_SEQ_POSTFIX: [%s]\n", ID_SEQ_POSTFIX.toStdString().c_str());
    }
	else {
		LOG_ERROR("ID_SEQ_POSTFIX not set in config file.\n");
		exit(1);
	}
    if(vm.count("SKIP_NONEXPLOIT_OUTPUT")) {
		SKIP_NONEXPLOIT_OUTPUT = vm["SKIP_NONEXPLOIT_OUTPUT"].as<bool>();
        LOG("SKIP_NONEXPLOIT_OUTPUT: [%s]\n", (SKIP_NONEXPLOIT_OUTPUT) ? "true" : "false");
    }
	else {
        LOG_ERROR("SKIP_NONEXPLOIT_OUTPUT not set in config file\n");
		exit(1);
	}

    LOG("SUCCESS\n");
}
