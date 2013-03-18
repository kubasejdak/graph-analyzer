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
#include <utils/SystemLogger.h>

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
			("version,v", "print version")
			("task,t", "execute system tasks" )
			;

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, options), vm);
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
	/* call rpc function */
	if(vm.count("rpc")) {
		bool stat = system.executeTasks();

		if(stat == false) {
			LOG("Executing tasks failed\n");
			return 1;
		}
	}

    return 0;
}
