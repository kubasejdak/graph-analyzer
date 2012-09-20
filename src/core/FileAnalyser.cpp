/*
 * Filename		: FileAnalyser.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "FileAnalyser.h"

FileAnalyser::FileAnalyser()
{
	type = "binary";
}

FileAnalyser::~FileAnalyser()
{
}

string FileAnalyser::analyze(string filename)
{
	file.open(filename.c_str(), fstream::in | fstream::binary);
	if(!file.is_open())
		return "";

	/* Windows executable */
	if(checkExe())
		goto exit;

	/* pcap */
	if(checkPcap())
		goto exit;

exit:
	file.close();
	return type;
}

bool FileAnalyser::checkExe()
{
	char buff[2];
	file.read(buff, 2);
	file.seekg(0, ios::beg);

	if(strcmp(buff, "MZ") != 0)
		return false;

	type = "exe";
	return true;
}

bool FileAnalyser::checkPcap()
{
	byte_t buff[4];
	file.read((char *) buff, 4);
	file.seekg(0, ios::beg);

	uint32_t magic_number = 0;
	magic_number += buff[3] << 24;
	magic_number += buff[2] << 16;
	magic_number += buff[1] << 8;
	magic_number += buff[0];

	if(magic_number != 0xa1b2c3d4 && magic_number != 0xd4c3b2a1)
		return false;

	type = "pcap";
	return true;
}
