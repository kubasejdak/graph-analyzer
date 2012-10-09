/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

/* standard headers */
#include <boost/program_options.hpp>
namespace opt = boost::program_options;

#include <iostream>
#include <list>
#include <iomanip>
using namespace std;

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <qt4/QtSql/QtSql>

/* project headers */
#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <core/SystemLogger.h>
#include <core/Options.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
#include <toolbox.h>

/* global variables */
struct Options {
	vector<string> input, output;
	string log_file;
	int log_level;
} sys_options;

/* console printing */
void printIntro(string version);
void printModuleInfo(ModuleInfo *info);
void listInputMods();
void listAnalyzeMods();
void listOutputMods();

/* database connections */
bool dbUpdateSystemInfo(string version, string status, string error);
bool dbGetOptions();
bool dbGetPendingFiles();

int main(int argc, char *argv[])
{
	/* create main system */
	CoreSystem system;
	system.setLogLevel(1);
	system.setLogFile("/home/kuba/analyzer_log");

	/* parse cmd line arguments */
	opt::options_description options("Options");
	options.add_options()
		("help,h", "print help message")
		("slave,s", "run program as GUI slave")
		("input,i", opt::value<vector<string> >(&sys_options.input), "set input files")
		("output,o", opt::value<vector<string> >(&sys_options.output), "set output destinations (default: ConsoleOutput)")
		("list-analyze", "list analyze modules")
		("list-input", "list input modules")
		("list-output", "list output modules")
		("version,v", "print version")
		("update-db,u", "update database with system info")
		("log-file", opt::value<string>(&sys_options.log_file), "set file to save logs")
		("log-level", opt::value<int>(&sys_options.log_level), "set logging level")
	;

	opt::positional_options_description p;
	p.add("input", -1);

	opt::variables_map vm;
	opt::store(opt::command_line_parser(argc, argv).options(options).positional(p).run(), vm);
	opt::notify(vm);

	/* check any arguments */
	if(vm.empty()) {
		LOG_ERROR("no arguments\n");
		return 1;
	}
	/* help */
	if(vm.count("help")) {
		cout << options << endl;
		return 0;
	}
	/* list-analyze */
	if(vm.count("list-analyze")) {
		listAnalyzeMods();
		return 0;
	}
	/* list-input */
	if(vm.count("list-input")) {
		listInputMods();
		return 0;
	}
	/* list-output */
	if(vm.count("list-output")) {
		listOutputMods();
		return 0;
	}
	/* version */
	if(vm.count("version")) {
		cout << system.getVersion() << endl;
		return 0;
	}
	/* update database */
	if(vm.count("update-db")) {
		bool ok = dbUpdateSystemInfo(system.getVersion(), system.getStatus(), system.getError());
		return (ok) ? 0 : 1;
	}
	/* run as GUI slave */
	if(vm.count("slave")) {
		bool ok = dbGetOptions();
		if(!ok)
			return 1;
	}
	else {
		/* log-file */
		if(vm.count("log-file"))
			system.setLogFile(sys_options.log_file);
		/* log-level */
		if(vm.count("log-level"))
			system.setLogLevel(sys_options.log_level);
		/* input */
		if(!vm.count("input")) {
			LOG_ERROR("no input files\n");
			exit(1);
		}
		/* output */
		if(!vm.count("output")) {
			LOG("no output destination, setting to ConsoleOutput");
			system.setOutput("ConsoleOutput");
		}
		while(!sys_options.output.empty()) {
			system.setOutput(sys_options.output.back());
			sys_options.output.pop_back();
		}
	}

	/* run */
	printIntro(system.getVersion());
	while(!sys_options.input.empty()) {
		system.addFile(sys_options.input.back());
		sys_options.input.pop_back();
		system.run();
		system.clear();
	}

	int s_num = system.getSamplesNum();
	int e_num = system.getExploitsNum();
	string e = (e_num == 1) ? "exploit" : "exploits";
	string s = (s_num == 1) ? "sample" : "samples";
	cout << "FINISHED: found " << dec << e_num << " " << e << " in " << s_num << " " << s << "!" << endl;

	return 0;
}

void printIntro(string version)
{
	cout << "graph-analyzer " << version << endl;
	cout << "Author: " << AUTHOR << endl;
	cout << "Build date: " << __DATE__ << endl;
	cout << endl;
}

void printModuleInfo(ModuleInfo *info)
{
	cout << "id: " << setw(6) << left << info->id << " ";
	cout << "name: " << setw(17) << left << info->name << " ";
	cout << "description: " << info->description << endl;
}

void listInputMods()
{
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listInput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;

	cout << "* List of available input modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}

void listAnalyzeMods()
{
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listAnalyze();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "* List of available analyze modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}

void listOutputMods()
{
	list<ModuleInfo *> mods = ModuleManager::getInstance()->listOutput();
	if(mods.size() == 0)
		return;

	list<ModuleInfo *>::iterator it;
	cout << "* List of available output modules:" << endl;
	for(it = mods.begin(); it != mods.end(); ++it)
		printModuleInfo((*it));

	cout << endl;
}

bool dbUpdateSystemInfo(string version, string status, string error)
{
	QSqlDatabase db = QSqlDatabase::addDatabase(DB_QT_DRIVER.c_str());
	db.setHostName(DB_HOST.c_str());
	db.setDatabaseName(DB_NAME.c_str());
	db.setUserName(DB_USER.c_str());
	db.setPassword(DB_PASS.c_str());

	bool db_opened = db.open();
	if(db_opened) {
		QSqlQuery select_query;
		select_query.prepare("SELECT * FROM options_systeminfo");
		if(!select_query.exec()) {
			LOG_ERROR("%s\n", select_query.lastError().databaseText().toStdString().c_str());
			db.close();
			return false;
		}
		bool record_exists = select_query.next();

		/* there is record in db, only update is necessary */
		if(record_exists) {
			QSqlQuery update_query;
			int id = update_query.record().value("id").toInt();
			update_query.prepare("UPDATE options_systeminfo SET id = ?, version = ?, status = ?, error = ? WHERE id = ?");
			update_query.bindValue(0, id);
			update_query.bindValue(1, version.c_str());
			update_query.bindValue(2, status.c_str());
			update_query.bindValue(3, error.c_str());
			update_query.bindValue(4, id);
			if(!update_query.exec()) {
				LOG_ERROR("%s\n", update_query.lastError().databaseText().toStdString().c_str());
				db.close();
				return false;
			}
		}
		/* no record in db, create a new one */
		else {
			/* get next id number */
			QSqlQuery seq_query;
			seq_query.prepare("SELECT nextval('options_systeminfo_id_seq')");
			if(!seq_query.exec()) {
				LOG_ERROR("%s\n", seq_query.lastError().databaseText().toStdString().c_str());
				db.close();
				return false;
			}
			seq_query.next();
			int id = seq_query.record().value("nextval").toInt();

			QSqlQuery insert_query;
			insert_query.prepare("INSERT INTO options_systeminfo VALUES (?, ?, ?, ?)");
			insert_query.bindValue(0, id);
			insert_query.bindValue(1, version.c_str());
			insert_query.bindValue(2, status.c_str());
			insert_query.bindValue(3, error.c_str());
			if(!insert_query.exec()) {
				LOG_ERROR("%s\n", insert_query.lastError().databaseText().toStdString().c_str());
				db.close();
				return false;
			}
		}
		db.close();
		return true;
	} /* if db_opened */

	return false;
}

bool dbGetOptions()
{
	QSqlDatabase db = QSqlDatabase::addDatabase(DB_QT_DRIVER.c_str());
	db.setHostName(DB_HOST.c_str());
	db.setDatabaseName(DB_NAME.c_str());
	db.setUserName(DB_USER.c_str());
	db.setPassword(DB_PASS.c_str());

	bool db_opened = db.open();
	if(db_opened) {
		/* get options */
		QSqlQuery options_query;
		options_query.prepare("SELECT * FROM options_option");
		if(!options_query.exec()) {
			LOG_ERROR("%s\n", options_query.lastError().databaseText().toStdString().c_str());
			db.close();
			return false;
		}
		options_query.next();
		sys_options.log_file = options_query.record().value("log_file").toString().toStdString();
		sys_options.log_level = options_query.record().value("log_level").toInt();
		sys_options.output.push_back(options_query.record().value("output_dest").toString().toStdString());

		/* get pending files */
		QSqlQuery files_query;
		files_query.prepare("SELECT * FROM options_pendingfile");
		if(!files_query.exec()) {
			LOG_ERROR("%s\n", files_query.lastError().databaseText().toStdString().c_str());
			db.close();
			return false;
		}

		while(files_query.next())
			sys_options.input.push_back(files_query.record().value("name").toString().toStdString());

		db.close();
		return true;
	}  /* if db_opened */

	return false;
}
