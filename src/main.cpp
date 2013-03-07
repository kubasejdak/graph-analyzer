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
#include <core/RPCTagResolver.h>
#include <utils/SystemLogger.h>

/* global variables */
vector<string> input;

int main(int argc, char *argv[])
{
	/* create main system */
	CoreSystem system;

	/* parse cmd line arguments */
	opt::options_description options("Options");
	string rpcFunc;
	options.add_options()
			("help,h", "print help message")
			("version,v", "print version")
			("rpc,r",opt::value<string>(&rpcFunc), "call system function" )
			("list-rpc,l", "print all available system functions")
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
	/* list rpc functions */
	if(vm.count("list-rpc")) {
		RPCTagResolver rpcResolver;
		QList<QString> calls = rpcResolver.availableCalls();
		for(int i = 0; i < calls.size(); ++i)
			cout << calls.at(i).toStdString().c_str() << endl;

		return 0;
	}
	/* call rpc function */
	if(vm.count("rpc")) {
		bool stat = system.execute(rpcFunc.c_str());

		if(stat == false) {
			LOG("RPC function call failed: [%s]\n", rpcFunc.c_str());
			return 1;
		}
	}

    return 0;
}
