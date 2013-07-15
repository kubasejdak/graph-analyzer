/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2013
 */

#include "DatabaseOutput.h"

#include <sstream>
#include <QSqlQuery>

#include <utils/SystemLogger.h>
#include <utils/DatabaseManager.h>

using namespace std;
using namespace Group;

DatabaseOutput::DatabaseOutput()
{
	m_name = "database";
	m_description = "Uploads info about groups in database.";
}

bool DatabaseOutput::exportOutput(ExploitGroupHandle group, int taskId, bool duplicate)
{
	// get next group_id number
	int groupId = DatabaseManager::instance()->sequenceValue("group_group_id_seq");

	// general group data
	if(exportGeneralData(group, groupId, taskId) == false) {
		LOG_ERROR("exporting general group info to database\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	// general members data
	if(exportMembersData(group, groupId) == false) {
		LOG_ERROR("exporting group members info to database\n");
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::exportGeneralData(ExploitGroupHandle group, int groupId, int taskId)
{
	LOG("exporting group info [general] to database\n");
	stringstream ss;
	ss << "INSERT INTO group_group VALUES (" << groupId
											 << ", " << DatabaseManager::instance()->sampleId(group->get(0))
											 << ", " << group->size()
											 << ", " << "''"
											 << ", " << taskId << ")";

	QSqlQuery sampleQuery(DatabaseManager::instance()->database());
	sampleQuery.prepare(ss.str().c_str());
	if(!DatabaseManager::instance()->exec(&sampleQuery)) {
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::exportMembersData(ExploitGroupHandle group, int groupId)
{
	LOG("exporting group info [members] to database\n");

	for(int i = 0; i < group->size(); ++i) {
		// get next groupassignment_id number
		int assignmentId = DatabaseManager::instance()->sequenceValue("group_groupassignment_id_seq");

		stringstream ss;
		ExploitSampleHandle sample = group->get(i);
		int resemblence = group->resemblence(i);
		ss << "INSERT INTO group_groupassignment VALUES (" << assignmentId
														   << ", " << groupId
														   << ", " << DatabaseManager::instance()->sampleId(sample)
														   << ", " << resemblence << ")";

		QSqlQuery sampleQuery(DatabaseManager::instance()->database());
		sampleQuery.prepare(ss.str().c_str());
		if(!DatabaseManager::instance()->exec(&sampleQuery)) {
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
