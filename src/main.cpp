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
#include <modules/ModuleInfo.h>
#include <modules/ModuleManager.h>

/* global variables */
struct Options {
    vector<string> input, output;
    string logFile;
    int logLevel;
} sysOptions;

/* console printing */
void printIntro(QString version);
void printModuleInfo(ModuleInfo *info);
void listInputMods();
void listAnalyzeMods();
void listOutputMods();

/* database connections */
bool dbUpdateSystemInfo(QString version, QString status, QString error, int progress, int exploits, int samples);
bool dbGetOptions();
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
            ("slave,s", "run program as GUI slave")
            ("input,i", opt::value<vector<string> >(&sysOptions.input), "set input files")
            ("output,o", opt::value<vector<string> >(&sysOptions.output), "set output destinations (default: ConsoleOutput)")
            ("list-analyze", "list analyze modules")
            ("list-input", "list input modules")
            ("list-output", "list output modules")
            ("version,v", "print version")
            ("update-db,u", "update database with system info")
            ("log-file", opt::value<string>(&sysOptions.logFile), "set file to save logs")
            ("log-level", opt::value<int>(&sysOptions.logLevel), "set logging level")
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
        cout << system.version().toStdString() << endl;
        return 0;
    }
    /* update database */
    if(vm.count("update-db")) {
        dbDropRecentFiles();
        bool ok = dbUpdateSystemInfo(system.version(), system.status(), system.error(), 0, 0, 0);
        return (ok) ? 0 : 1;
    }
    /* run as GUI slave */
    if(vm.count("slave")) {
        bool ok = dbGetOptions();
        if(!ok)
            return 1;

        system.setLogFile(sysOptions.logFile.c_str());
        system.setLogLevel(sysOptions.logLevel);
        while(!sysOptions.output.empty()) {
            system.setOutput(sysOptions.output.back().c_str());
            sysOptions.output.pop_back();
        }
    }
    /* run in normal mode */
    else {
        /* log-file */
        if(vm.count("log-file"))
            system.setLogFile(sysOptions.logFile.c_str());
        /* log-level */
        if(vm.count("log-level"))
            system.setLogLevel(sysOptions.logLevel);
        /* input */
        if(!vm.count("input")) {
            LOG_ERROR("no input files\n");
            return 1;
        }
        /* output */
        if(!vm.count("output")) {
            LOG("no output destination, setting to DatabaseOutput\n");
            system.setOutput("DatabaseOutput");
        }
        while(!sysOptions.output.empty()) {
            system.setOutput(sysOptions.output.back().c_str());
            sysOptions.output.pop_back();
        }
    }

    /* run */
    LOG("starting system\n");
    dbDropRecentFiles();
    dbUpdateSystemInfo(system.version(), system.status(), system.error(), 0, 0, 0);
    int begSize = sysOptions.input.size();
    printIntro(system.version());
    while(!sysOptions.input.empty()) {
        system.addFile(sysOptions.input.back().c_str());
        if(vm.count("slave")) {
            dbRemoveFile(sysOptions.input.back().c_str());
            dbAddRecentFile(sysOptions.input.back().c_str());
        }
        sysOptions.input.pop_back();
        system.run();
        LOG("file processing completed\n");
        double perc = ((begSize - sysOptions.input.size()) / begSize) * 100;
        if(vm.count("slave"))
            dbUpdateSystemInfo(system.version(), system.status(), system.error(), (int) perc, system.exploitsNum(), system.samplesNum());
    }

    int s_num = system.samplesNum();
    int e_num = system.exploitsNum();
    string e = (e_num == 1) ? "exploit" : "exploits";
    string s = (s_num == 1) ? "sample" : "samples";
    cout << "FINISHED: found " << dec << e_num << " " << e << " in " << s_num << " " << s << "!" << endl;
    LOG("program FINISHED without error\n");

    return 0;
}

void printIntro(QString version)
{
    cout << "graph-analyzer " << version.toStdString() << endl;
    cout << "Author: " << AUTHOR << endl;
    cout << "Build date: " << __DATE__ << endl;
    cout << endl;
}

void printModuleInfo(ModuleInfo *info)
{
    cout << "id: " << setw(6) << left << info->m_id << " ";
    cout << "name: " << setw(17) << left << info->m_name.toStdString() << " ";
    cout << "description: " << info->m_description.toStdString() << endl;
}

void listInputMods()
{
    QList<ModuleInfo *> mods = ModuleManager::instance()->listInput();
    if(mods.size() == 0)
        return;

    QList<ModuleInfo *>::iterator it;
    cout << "* List of available input modules:" << endl;
    for(it = mods.begin(); it != mods.end(); ++it)
        printModuleInfo(*it);

    cout << endl;
}

void listAnalyzeMods()
{
    QList<ModuleInfo *> mods = ModuleManager::instance()->listAnalyze();
    if(mods.size() == 0)
        return;

    QList<ModuleInfo *>::iterator it;
    cout << "* List of available analyze modules:" << endl;
    for(it = mods.begin(); it != mods.end(); ++it)
        printModuleInfo(*it);

    cout << endl;
}

void listOutputMods()
{
    QList<ModuleInfo *> mods = ModuleManager::instance()->listOutput();
    if(mods.size() == 0)
        return;

    QList<ModuleInfo *>::iterator it;
    cout << "* List of available output modules:" << endl;
    for(it = mods.begin(); it != mods.end(); ++it)
        printModuleInfo(*it);

    cout << endl;
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

bool dbGetOptions()
{
    /* get options */
    QSqlQuery options_query(DatabaseManager::instance()->database());
    options_query.prepare("SELECT * FROM options_option");
    if(!DatabaseManager::instance()->exec(&options_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }

    options_query.next();
    sysOptions.logFile = options_query.record().value("log_file").toString().toStdString();
    sysOptions.logLevel = options_query.record().value("log_level").toInt();
    sysOptions.output.push_back(options_query.record().value("output_dest").toString().toStdString());

    /* get pending files */
    QSqlQuery files_query(DatabaseManager::instance()->database());
    files_query.prepare("SELECT * FROM options_pendingfile");
    if(!DatabaseManager::instance()->exec(&files_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        return false;
    }

    while(files_query.next())
        sysOptions.input.push_back(files_query.record().value("name").toString().toStdString());

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
