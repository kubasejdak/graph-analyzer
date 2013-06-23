/*
 *  Filename	: UniqueNameResolver.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "UniqueNameResolver.h"

#include <string>
#include <sstream>
#include <QFile>
#include <QFileInfo>

#include <utils/SystemLogger.h>
#include <utils/Toolbox.h>

using namespace std;

string UniqueNameResolver::resolve(string filename)
{
	string uniqueName = filename;
	int k = 2;
	while(QFile(uniqueName.c_str()).exists()) {
		LOG("duplicate for given filename found, renaming\n");
		QFileInfo duplicate(uniqueName.c_str());
		stringstream ss;
		ss << duplicate.absolutePath().toStdString() << "/" << duplicate.baseName().toStdString();
		uniqueName = ss.str();

		if(k != 2) {
			int n = uniqueName.find_last_of('_');
			uniqueName = uniqueName.substr(0, n + 1);
		}
		else
			uniqueName += "_";

		uniqueName += Toolbox::itos(k) + ".png";
		LOG("renamed to: [%s]\n", uniqueName.c_str());
		++k;
	}

	LOG("SUCCESS\n\n");
	return uniqueName;
}
