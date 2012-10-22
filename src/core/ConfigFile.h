/*
 * Filename		: ConfigFile.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <QString>
#include <string>
#include <boost/program_options.hpp>
using namespace std;
namespace opt = boost::program_options;

#include <core/Options.h>
#include <core/SystemLogger.h>
#include <core/Toolbox.h>

class ConfigFile {
public:
    static ConfigFile *instance()
	{
        static ConfigFile obj;
        return &obj;
	}

	void read();

private:
    ConfigFile() {}
};

#endif /* CONFIGFILE_H_ */
