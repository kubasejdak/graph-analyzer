/*
 *  Filename	: main.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#define AUTHOR	"Kuba Sejdak"

#include <boost/program_options.hpp>
namespace opt = boost::program_options;

#include <QString>
#include <QList>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include <core/CoreSystem.h>
#include <core/SystemLogger.h>

/* global variables */
vector<string> input;

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
		bool ok = system.dbUpdateSystemInfo();
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
	system.dbUpdateSystemInfo();

	system.run();

	system.dbUpdateSystemInfo();
	LOG("stopping system\n");
	/* ====== run finished  ====== */

	LOG("FINISHED: found %d exploit(s) in %d sample(s) extracted from %d file(s)!\n", system.exploitsNum(), system.samplesNum(), system.filesNum());
	if(system.errorsNum())
		LOG("some ERRORS occured, errorCounter: [%d]\n", system.errorsNum());
    else
		LOG("no errors occured\n");

    return 0;
}
