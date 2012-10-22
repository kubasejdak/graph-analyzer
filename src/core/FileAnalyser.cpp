/*
 * Filename		: FileAnalyser.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "FileAnalyser.h"

FileAnalyser::FileAnalyser()
{
    m_type = "binary";
}

FileAnalyser::~FileAnalyser()
{
}

QString FileAnalyser::analyze(QString filename)
{
    m_file.setFileName(filename);
    if(!m_file.open(QIODevice::ReadOnly))
		return "";

	/* Windows executable */
	if(checkExe())
        goto exit;
	/* pcap */
	if(checkPcap())
		goto exit;

exit:
    m_file.close();
    return m_type;
}

bool FileAnalyser::checkExe()
{
	char buff[2];
    m_file.read(buff, 2);
    m_file.seek(0);

	if(strcmp(buff, "MZ") != 0)
		return false;

    m_type = "exe";
	return true;
}

bool FileAnalyser::checkPcap()
{
	byte_t buff[4];
    m_file.read((char *) buff, 4);
    m_file.seek(0);

	uint32_t magic_number = 0;
	magic_number += buff[3] << 24;
	magic_number += buff[2] << 16;
	magic_number += buff[1] << 8;
	magic_number += buff[0];

	if(magic_number != 0xa1b2c3d4 && magic_number != 0xd4c3b2a1)
		return false;

    m_type = "pcap";
	return true;
}
