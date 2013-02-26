/*
 * Filename		: Options.cpp
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#include <core/Options.h>
#include <utils/DatabaseManager.h>

Options::Options()
{
	/* database options (taken from config file */
	DB_QT_DRIVER = "QPSQL";
	DB_HOST = "localhost";
	DB_NAME = "graph_analyzer_db";
	DB_USER = "jsejdak";
	DB_PASS = "poliritiper";
	ANA_TABLE_PREFIX = "analyze_";
	OPT_TABLE_PREFIX = "option_";
	ID_SEQ_POSTFIX = "_id_seq";

	/* emulation options */
	EMULATION_STEPS = 1000000;
	MAX_INPUT_FILE_SIZE = 20971520;
	RESEMBLENCE_LEVEL = 50;
	SKIP_NONEXPLOIT_OUTPUT = true;
	PROTECT_AGAINST_BIG_FILES = true;
	SKIP_BROKEN_SAMPLES = true;
	BROKEN_SAMPLE_SIZE = 30;
	GRAPHS_DIR = "/var/www/jsejdak/graphs";
}

bool Options::readConfigFile()
{
	LOG("reading config file\n");

	QFile confFile(CONFIG_FILE);
	if(!confFile.exists())
		return false;

	opt::options_description config("config");
	config.add_options()
		("DB_QT_DRIVER", opt::value<string>(), "Qt driver for database")
		("DB_HOST", opt::value<string>(), "database host")
		("DB_NAME", opt::value<string>(), "database name")
		("DB_USER", opt::value<string>(), "database user")
		("DB_PASS", opt::value<string>(), "database password")
		("ANA_TABLE_PREFIX", opt::value<string>(), "prefix for all analyze tables in database")
		("OPT_TABLE_PREFIX", opt::value<string>(), "prefix for all options tables in database")
		("ID_SEQ_POSTFIX", opt::value<string>(), "postfix for all GraphAnalyzer sequences in database");
	opt::variables_map vm;

	opt::store(opt::parse_config_file<char>(CONFIG_FILE, config), vm);
	opt::notify(vm);

	if(vm.count("DB_QT_DRIVER"))
		DB_QT_DRIVER = vm["DB_QT_DRIVER"].as<string>().c_str();
	else
		return false;

	if(vm.count("DB_HOST"))
		DB_HOST = vm["DB_HOST"].as<string>().c_str();
	else
		return false;

	if(vm.count("DB_NAME"))
		DB_NAME = vm["DB_NAME"].as<string>().c_str();
	else
		return false;

	if(vm.count("DB_USER"))
		DB_USER = vm["DB_USER"].as<string>().c_str();
	else
		return false;

	if(vm.count("DB_PASS"))
		DB_PASS = vm["DB_PASS"].as<string>().c_str();
	else
		return false;

	if(vm.count("ANA_TABLE_PREFIX"))
		ANA_TABLE_PREFIX = vm["ANA_TABLE_PREFIX"].as<string>().c_str();
	else
		return false;

	if(vm.count("OPT_TABLE_PREFIX"))
		OPT_TABLE_PREFIX = vm["OPT_TABLE_PREFIX"].as<string>().c_str();
	else
		return false;

	if(vm.count("ID_SEQ_POSTFIX"))
		ID_SEQ_POSTFIX = vm["ID_SEQ_POSTFIX"].as<string>().c_str();
	else
		return false;

	return true;
}

bool Options::readDatabaseOptions()
{
	/* get options */
	QSqlQuery options_query(DatabaseManager::instance()->database());
	options_query.prepare("SELECT * FROM options_option");
	if(!DatabaseManager::instance()->exec(&options_query))
		return false;

	if(!options_query.next())
		return false;

	/* logging */
	SystemLogger::instance()->setLogLevel(options_query.record().value("log_level").toInt());
	SystemLogger::instance()->setLogFile(options_query.record().value("log_file").toString());

	/* system */
	RESEMBLENCE_LEVEL = options_query.record().value("resemblence_level").toInt();
	EMULATION_STEPS = options_query.record().value("emulation_steps").toInt();
	GRAPHS_DIR = options_query.record().value("graphs_dir").toString();
	PROTECT_AGAINST_BIG_FILES = options_query.record().value("big_files_protection").toBool();
	MAX_INPUT_FILE_SIZE = options_query.record().value("max_input_file_size").toInt();
	SKIP_NONEXPLOIT_OUTPUT = options_query.record().value("skip_empty_samples").toBool();
	SKIP_BROKEN_SAMPLES = options_query.record().value("skip_broken_samples").toBool();
	BROKEN_SAMPLE_SIZE = options_query.record().value("broken_samples_size").toInt();

	/* get pending files */
	QSqlQuery files_query(DatabaseManager::instance()->database());
	files_query.prepare("SELECT * FROM options_pendingfile");
	if(!DatabaseManager::instance()->exec(&files_query))
		return false;

	while(files_query.next())
		pendingFiles.push_back(files_query.record().value("name").toString());

	return true;
}

void Options::listOptions()
{
	/* database settings */
	LOG("database settings:\n");
	LOG("DB_QT_DRIVER: [%s]\n", DB_QT_DRIVER.toStdString().c_str());
	LOG("DB_HOST: [%s]\n", DB_HOST.toStdString().c_str());
	LOG("DB_NAME: [%s]\n", DB_NAME.toStdString().c_str());
	LOG("DB_USER: [%s]\n", DB_USER.toStdString().c_str());
	LOG("DB_PASS: [%s]\n", DB_PASS.toStdString().c_str());
	LOG("ANA_TABLE_PREFIX: [%s]\n", ANA_TABLE_PREFIX.toStdString().c_str());
	LOG("OPT_TABLE_PREFIX: [%s]\n", OPT_TABLE_PREFIX.toStdString().c_str());
	LOG("ID_SEQ_POSTFIX: [%s]\n", ID_SEQ_POSTFIX.toStdString().c_str());

	/* system settings */
	LOG("system settings:\n");
	LOG("RESEMBLENCE_LEVEL: [%d]\n", RESEMBLENCE_LEVEL);
	LOG("EMULATION_STEPS: [%d]\n", EMULATION_STEPS);
	LOG("GRAPHS_DIR: [%s]\n", GRAPHS_DIR.toStdString().c_str());
	LOG("PROTECT_AGAINST_BIG_FILES: [%s]\n", PROTECT_AGAINST_BIG_FILES ? "true" : "false");
	LOG("MAX_INPUT_FILE_SIZE: [%d]\n", MAX_INPUT_FILE_SIZE);
	LOG("SKIP_NONEXPLOIT_OUTPUT: [%s]\n", SKIP_NONEXPLOIT_OUTPUT ? "true" : "false");
	LOG("SKIP_BROKEN_SAMPLES: [%s]\n", SKIP_BROKEN_SAMPLES ? "true" : "false");
	LOG("BROKEN_SAMPLE_SIZE: [%d]\n", BROKEN_SAMPLE_SIZE);

	/* pending files */
	LOG("pending files (from database):\n");
	for(int i = 0; i < pendingFiles.size(); ++i)
		LOG("file: [%s]\n", pendingFiles.at(i).toStdString().c_str());
}
