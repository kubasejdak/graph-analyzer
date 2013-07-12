/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2013
 */

#include "DatabaseOutput.h"

using namespace Group;

DatabaseOutput::DatabaseOutput()
{
	m_name = "database";
	m_description = "Uploads info about groups in database.";
}

bool DatabaseOutput::exportOutput(ExploitGroupHandle sample, int taskId, bool override)
{
	return true;
}
