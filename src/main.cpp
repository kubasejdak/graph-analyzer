/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

/* standard headers */
#include <boost/program_options.hpp>

#include <iostream>
#include <list>
#include <iomanip>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <qt4/QtCore/QtCore>
#include <qt4/QtSql/QtSql>
//#include <QtSql>
using namespace std;
namespace opt = boost::program_options;

/* project headers */
#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <core/SystemLogger.h>
#include <core/Options.h>
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>
#include <toolbox.h>

void printIntro(string version);
void printModuleInfo(ModuleInfo *info);
void listInputMods();
void listAnalyzeMods();
void listOutputMods();

int main(int argc, char *argv[])
{
	/* create main system */
	CoreSystem system;
	system.setLogLevel(1);
	system.setLogFile("/home/kuba/analyzer_log");

	/* parse cmd line arguments */
	vector<string> input, output;
	string log_file, log_level;
	opt::options_description options("Options");
	options.add_options()
		("help,h", "print help message")
		("slave,s", "run program as GUI slave")
		("input,i", opt::value<vector<string> >(&input), "set input files")
		("output,o", opt::value<vector<string> >(&output), "set output destinations (default: ConsoleOutput)")
		("list-analyze", "list analyze modules")
		("list-input", "list input modules")
		("list-output", "list output modules")
		("version,v", "print version")
		("update-db,u", "update database with system info")
		("log-file", opt::value<string>(&log_file), "set file to save logs")
		("log-level", opt::value<string>(&log_level), "set logging level")
	;

	opt::positional_options_description p;
	p.add("input", -1);

	opt::variables_map vm;
	opt::store(opt::command_line_parser(argc, argv).options(options).positional(p).run(), vm);
	opt::notify(vm);

	/* check any arguments */
	if(vm.empty()) {
		LOG_ERROR("no arguments\n");
		exit(1);
	}
	/* help */
	if(vm.count("help")) {
		cout << options << endl;
		exit(0);
	}
	/* list-analyze */
	if(vm.count("list-analyze")) {
		listAnalyzeMods();
		exit(0);
	}
	/* list-input */
	if(vm.count("list-input")) {
		listInputMods();
		exit(0);
	}
	/* list-output */
	if(vm.count("list-output")) {
		listOutputMods();
		exit(0);
	}
	/* version */
	if(vm.count("version")) {
		cout << system.getVersion() << endl;
		exit(0);
	}
	/* update database */
	if(vm.count("update-db")) {
		string version = system.getVersion();
		string status = system.getStatus();
		string error = system.getError();

		QQSqlDatabase db = QSqlDatabase::addDatabase(DB_QT_DRIVER.c_str());
		db.setHostName(DB_HOST.c_str());
		db.setDatabaseName(DB_NAME.c_str());
		db.setUserName(DB_USER.c_str());
		db.setPassword(DB_PASS.c_str());
		bool db_opened = db.open();
		if(db_opened) {
			QSqlQuery query;
			QSqlQuery update_query;
			QSqlQuery insert_query;
			QSqlQuery seq_query;

			query.prepare("SELECT * FROM options_systeminfo");
			if(!query.exec()) {
				LOG_ERROR("%s\n", query.lastError().databaseText().toStdString().c_str());
				db.close();
				exit(1);
			}
			bool record_exists = query.next();

			/* there is record in db, only update is necessary */
			if(record_exists) {
				int id = query.record().value("id").toInt();
				update_query.prepare("UPDATE options_systeminfo SET id = ?, version = ?, status = ?, error = ? WHERE id = ?");
				update_query.bindValue(0, id);
				update_query.bindValue(1, version.c_str());
				update_query.bindValue(2, status.c_str());
				update_query.bindValue(3, error.c_str());
				update_query.bindValue(4, id);
				if(!update_query.exec()) {
					LOG_ERROR("%s\n", update_query.lastError().databaseText().toStdString().c_str());
					db.close();
					exit(1);
				}
			}
			/* no record in db, create a new one */
			else {
				/* get next id number */
				seq_query.prepare("SELECT nextval('options_systeminfo_id_seq')");
				if(!seq_query.exec()) {
					LOG_ERROR("%s\n", seq_query.lastError().databaseText().toStdString().c_str());
					db.close();
					exit(1);
				}
				seq_query.next();
				int id = seq_query.record().value("nextval").toInt();
				insert_query.prepare("INSERT INTO options_systeminfo VALUES (?, ?, ?, ?)");
				insert_query.bindValue(0, id);
				insert_query.bindValue(1, version.c_str());
				insert_query.bindValue(2, status.c_str());
				insert_query.bindValue(3, error.c_str());
				if(!insert_query.exec()) {
					LOG_ERROR("%s\n", insert_query.lastError().databaseText().toStdString().c_str());
					db.close();
					exit(1);
				}
			}
			db.close();
		} /* if db_opened */

		exit(0);
	}
	/* run as GUI slave */
	if(vm.count("slave")) {
		QSqlDatabase db = QSqlDatabase::addDatabase(DB_QT_DRIVER.c_str());
		db.setHostName(DB_HOST.c_str());
		db.setDatabaseName(DB_NAME.c_str());
		db.setUserName(DB_USER.c_str());
		db.setPassword(DB_PASS.c_str());
		bool db_opened = db.open();
		if(db_opened) {
			QSqlQuery options_query;
			QSqlQuery files_query;

			options_query.prepare("SELECT * FROM options_option");
			if(!options_query.exec()) {
				LOG_ERROR("%s\n", options_query.lastError().databaseText().toStdString().c_str());
				db.close();
				exit(1);
			}
			query.next();

			/* there is record in db, only update is necessary */
			int id = query.record().value("id").toInt();
			update_query.prepare("UPDATE options_systeminfo SET id = ?, version = ?, status = ?, error = ? WHERE id = ?");
			update_query.bindValue(0, id);
			update_query.bindValue(1, version.c_str());
			update_query.bindValue(2, status.c_str());
			update_query.bindValue(3, error.c_str());
			update_query.bindValue(4, id);
			if(!update_query.exec()) {
				LOG_ERROR("%s\n", update_query.lastError().databaseText().toStdString().c_str());
				db.close();
				exit(1);
			}

			db.close();
		} /* if db_opened */
		//TODO: poboerz log_file z db
		//TODO: pobierz log_level z db
		//TODO: pobierz output_dest z db
		//TODO: pobierz pending_files z db do input
	}
	else {
		/* log-file */
		if(vm.count("log-file"))
			system.setLogFile(log_file);
		/* log-level */
		if(vm.count("log-level"))
			system.setLogLevel(atoi(log_level.c_str()));
		/* input */
		if(!vm.count("input")) {
			LOG_ERROR("no input files\n");
			exit(1);
		}

		// TODO: nie dodawac tego na raz
		while(!input.empty()) {
			system.addFile(input.back());
			input.pop_back();
		}
		/* output */
		if(!vm.count("output")) {
			LOG("no output destination, setting to ConsoleOutput");
			system.setOutput("ConsoleOutput");
		}
		while(!output.empty()) {
			system.setOutput(output.back());
			output.pop_back();
		}
	}

	/* run */
	// TODO: dodawac sekwencyjnie, analizowac w jednym watku a w drugim sprawdzac w petli status
	printIntro(system.getVersion());
	system.run();

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
