/*
 *  Filename	: UniqueNameResolver.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "UniqueNameResolver.h"

#include <string>
#include <sstream>
#include <boost/filesystem.hpp>

#include <utils/SystemLogger.h>

using namespace std;

string UniqueNameResolver::resolve(string filename)
{
	boost::filesystem::path original = boost::filesystem::absolute(filename);

	string uniqueName = original.generic_string();

	boost::filesystem::path extension = original.extension();
	boost::filesystem::path stem = original.stem();
	boost::filesystem::path directory = original.remove_filename();

	boost::filesystem::path absolutePath = uniqueName;

	int k = 2;
	while(boost::filesystem::exists(absolutePath) == true) {
		LOG("duplicate for given filename found, renaming\n");

		stringstream ss;
		ss << directory.generic_string() << "/" << stem.generic_string() << "_" << k << extension.generic_string();
		uniqueName = ss.str();
		absolutePath = uniqueName;

		LOG("renamed to: [%s]\n", uniqueName.c_str());
		++k;
	}

	LOG("SUCCESS\n\n");
	return uniqueName;
}
