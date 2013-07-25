/*
 * Filename		: FileTypeAnalyzer.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "FileTypeAnalyzer.h"

#include <string>
#include <fstream>
#include <magic.h>

#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>

using namespace std;

string FileTypeAnalyzer::analyze(string filename)
{
	// try recognizing internal graph-analyzer type
	string fileType = recognizeInternalType(filename);

	// if not recognized, try MIME type with magic library
	if(fileType == "")
		fileType = recognizeMIMEType(filename);

    LOG("recognized file: [%s] as [%s] type\n", filename.c_str(), fileType.c_str());
	LOG("SUCCESS\n\n");
	return fileType;
}

string FileTypeAnalyzer::recognizeInternalType(string filename)
{
	string filetype = "";

	// open and read first line
	ifstream file;
	file.open(filename);
	string firstLine = "";
	file >> firstLine;

	// check if IndexFile
	if(firstLine == "#exploits_list")
		filetype = "graph-analyzer/index-file";

	// clean up
	file.close();
	return filetype;
}

string FileTypeAnalyzer::recognizeMIMEType(string filename)
{
	magic_t myt = magic_open(MAGIC_ERROR | MAGIC_MIME_TYPE);
	magic_load(myt, NULL);
	string fileType = magic_file(myt, filename.c_str());
	magic_close(myt);

	return fileType;
}
