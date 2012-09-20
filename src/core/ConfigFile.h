/*
 * Filename		: ConfigFile.h
 * Author		: Kuba Sejdak
 * Created on	: 16-09-2012
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

/* standard headers */
#include <boost/program_options.hpp>
namespace opt = boost::program_options;

/* project headers */
#include <core/Options.h>
#include <core/SystemLogger.h>
#include <toolbox.h>

class ConfigFile {
public:
	virtual ~ConfigFile();
	static ConfigFile *getInstance()
	{
		static ConfigFile instance;
		return &instance;
	}

	void read();

private:
	ConfigFile();
};

#endif /* CONFIGFILE_H_ */
