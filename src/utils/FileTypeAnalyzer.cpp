/*
 * Filename		: FileTypeAnalyzer.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "FileTypeAnalyzer.h"

#include <string>
#include <magic.h>

#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>

using namespace std;

string FileTypeAnalyzer::analyze(string filename)
{
	magic_t myt = magic_open(MAGIC_ERROR | MAGIC_MIME_TYPE);
	magic_load(myt, NULL);
    string fileType = magic_file(myt, filename.c_str());
	magic_close(myt);

    LOG("recognized file: [%s] as [%s] type\n", filename.c_str(), fileType.c_str());
	LOG("SUCCESS\n\n");
	return fileType;
}
