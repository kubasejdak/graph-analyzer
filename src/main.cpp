/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

#include <boost/program_options.hpp>
namespace opt = boost::program_options;

#include <QString>
#include <QtSql>
#include <QList>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

#include <core/CoreSystem.h>
#include <core/ShellcodeInfo.h>
#include <core/SystemLogger.h>
#include <core/Options.h>
#include <core/DatabaseManager.h>
#include <modules/ModuleManager.h>

/* global variables */
vector<string> input;

/* database connections */
bool dbUpdateSystemInfo(QString version, QString status, QString error, int progress, int exploits, int samples);
bool dbRemoveFile(QString file);
bool dbAddRecentFile(QString file);
bool dbDropRecentFiles();

int main(int argc, char *argv[])
{
    /* create main system */
    CoreSystem system;

    /* parse cmd line arguments */
    opt::options_description options("Options");
    options.add_options()
            ("help,h", "print help message")
			("input,i", opt::value<vector<string> >(&input), "set input files")
			("console,c", "add console to output destinations")
            ("version,v", "print version")
			("update-db,u", "update database with system info")
			("regroup", "regroup samples")
            ;

    opt::positional_options_description p;
    p.add("input", -1);

    opt::variables_map vm;
    opt::store(opt::command_line_parser(argc, argv).options(options).positional(p).run(), vm);
    opt::notify(vm);

    /* help */
    if(vm.count("help")) {
        cout << options << endl;
        return 0;
    }
    /* version */
    if(vm.count("version")) {
        cout << system.version().toStdString() << endl;
        return 0;
    }
    /* update database */
    if(vm.count("update-db")) {
        dbDropRecentFiles();
        bool ok = dbUpdateSystemInfo(system.version(), system.status(), system.error(), 0, 0, 0);
        return (ok) ? 0 : 1;
    }
	/* regroup samples */
	if(vm.count("regroup")) {
		system.makeGroups();
		return 0;
	}
	/* console */
	if(vm.count("console"))
		system.setOutput("ConsoleOutput");

	/* input from cmd */
	if(vm.count("input")) {
		while(!input.empty()) {
			string f = input.back();
			system.addFile(f.c_str());
			input.pop_back();
		}
	}

	/* ====== run ====== */
	LOG("starting system\n");

	int errorCounter = 0;
	double perc = 0.0;

    dbDropRecentFiles();
    dbUpdateSystemInfo(system.version(), system.status(), system.error(), 0, 0, 0);

	errorCounter = system.run();
	perc = 100.0;
	dbUpdateSystemInfo(system.version(), system.status(), system.error(), (int) perc, system.exploitsNum(), system.samplesNum());

	system.makeGroups();
	dbUpdateSystemInfo(system.version(), system.status(), system.error(), (int) perc, system.exploitsNum(), system.samplesNum());
	/* ====== run finished  ====== */

    int s_num = system.samplesNum();
    int e_num = system.exploitsNum();
    string e = (e_num == 1) ? "exploit" : "exploits";
    string s = (s_num == 1) ? "sample" : "samples";

	LOG("FINISHED: found %d %s in %d %s!\n", e_num, e.c_str(), s_num, s.c_str());

    if(errorCounter)
        LOG("program FINISHED with some ERRORS, errorCounter: [%d]\n", errorCounter);
    else
        LOG("program FINISHED without errors\n");

    return 0;
}

bool dbUpdateSystemInfo(QString version, QString status, QString error, int progress, int exploits, int samples)
{
    QSqlQuery select_query(DatabaseManager::instance()->database());
    select_query.prepare("SELECT * FROM options_systeminfo");
    if(!DatabaseManager::instance()->exec(&select_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }
    bool record_exists = select_query.next();

    /* there is record in db, only update is necessary */
    if(record_exists) {
        int id = select_query.record().value("id").toInt();
        QSqlQuery update_query(DatabaseManager::instance()->database());
        update_query.prepare("UPDATE options_systeminfo SET id = ?, version = ?, status = ?, error = ?, progress = ?, exploits_num = ?, samples_num = ? WHERE id = ?");
        update_query.addBindValue(id);
        update_query.addBindValue(version);
        update_query.addBindValue(status);
        update_query.addBindValue(error);
        update_query.addBindValue(progress);
        update_query.addBindValue(exploits);
        update_query.addBindValue(samples);
        update_query.addBindValue(id);
        if(!DatabaseManager::instance()->exec(&update_query)) {
            SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
            LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
            return false;
        }
    }
    /* no record in db, create a new one */
    else {
        /* get next id number */
        QSqlQuery seq_query(DatabaseManager::instance()->database());
        seq_query.prepare("SELECT nextval('options_systeminfo_id_seq')");
        if(!DatabaseManager::instance()->exec(&seq_query)) {
            SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
            LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
            return false;
        }
        seq_query.next();
        int id = seq_query.record().value("nextval").toInt();

        QSqlQuery insert_query(DatabaseManager::instance()->database());
        insert_query.prepare("INSERT INTO options_systeminfo VALUES (?, ?, ?, ?, ?, ?, ?)");
        insert_query.addBindValue(id);
        insert_query.addBindValue(version.toStdString().c_str());
        insert_query.addBindValue(status.toStdString().c_str());
        insert_query.addBindValue(error.toStdString().c_str());
        insert_query.addBindValue(progress);
        insert_query.addBindValue(exploits);
        insert_query.addBindValue(samples);
        if(!DatabaseManager::instance()->exec(&insert_query)) {
            SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
            LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
            return false;
        }
    } /* else */

    return true;
}

bool dbRemoveFile(QString file)
{
    /* remove file */
    QSqlQuery remove_query(DatabaseManager::instance()->database());
    remove_query.prepare("DELETE FROM options_pendingfile WHERE name = ?");
    remove_query.addBindValue(file);
    if(!DatabaseManager::instance()->exec(&remove_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }

    return true;
}

bool dbAddRecentFile(QString file)
{
    /* get next id number */
    QSqlQuery seq_query(DatabaseManager::instance()->database());
    seq_query.prepare("SELECT nextval('options_systeminfo_id_seq')");
    if(!DatabaseManager::instance()->exec(&seq_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }

    seq_query.next();
    int id = seq_query.record().value("nextval").toInt();

    /* add file */
    QSqlQuery insert_query(DatabaseManager::instance()->database());
    insert_query.prepare("INSERT INTO options_recentfile VALUES (?, ?)");
    insert_query.addBindValue(id);
    insert_query.addBindValue(file);
    if(!DatabaseManager::instance()->exec(&insert_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }

    return true;
}

bool dbDropRecentFiles()
{
    /* remove file */
    QSqlQuery delete_query(DatabaseManager::instance()->database());
    delete_query.prepare("DELETE FROM options_recentfile");
    if(!DatabaseManager::instance()->exec(&delete_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }

    return true;
}
