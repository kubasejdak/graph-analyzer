/*
 * Filename		: DatabaseOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 22-08-2012
 */

#include "DatabaseOutput.h"

DatabaseOutput::DatabaseOutput()
{
    m_name = "DatabaseOutput";
    m_description = "Inserts info about samples into database.";
}

bool DatabaseOutput::generateOutput(ExploitSample *sample)
{
    ExploitInfo *info = sample->info();

    /* ensure that sample is not a duplicate */
    bool duplicate = checkDuplicate(info);
    if(duplicate) {
        LOG("duplicate sample: skipping and removing duplicated graph file\n");
        QFile file(info->graphName());
        file.remove();
        LOG("SUCCESS\n\n");
        return true;
    }

	/* get next sample_id number */
	int sampleId = DatabaseManager::instance()->sequenceValue("analyze_sample_id_seq");

    /* general sample data */
	LOG("exporting sample info [general] to database\n");
	QSqlQuery sampleQuery(DatabaseManager::instance()->database());
	sampleQuery.prepare("INSERT INTO analyze_sample VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sampleQuery.addBindValue(sampleId);
	sampleQuery.addBindValue(info->name());
	sampleQuery.addBindValue(info->extractedFrom());
	sampleQuery.addBindValue(info->graphName());
	sampleQuery.addBindValue(QString().setNum(info->size()));
	sampleQuery.addBindValue(info->fileType());
	sampleQuery.addBindValue(QString().setNum(info->fileSize()));
	sampleQuery.addBindValue(QString().setNum(info->codeOffset()));
	sampleQuery.addBindValue("");
	if(!DatabaseManager::instance()->exec(&sampleQuery))
        return false;

    /* analyze modules specific data */
	AnalyzeMap *anaMods = ModuleManager::instance()->analyze();
	AnalyzeMap::iterator anaIt;
	for(anaIt = anaMods->begin(); anaIt != anaMods->end(); ++anaIt) {
		LOG("exporting sample info [%s] to database\n", anaIt.key().toStdString().c_str());
		anaIt.value()->exportToDatabase(sample, sampleId);
	}

	/* get next samplegroup_id number */
	int groupId = DatabaseManager::instance()->sequenceValue("analyze_samplegroup_id_seq");

	/* general samplegroup data */
	QSqlQuery groupQuery(DatabaseManager::instance()->database());
	groupQuery.prepare("INSERT INTO analyze_samplegroup VALUES (?, ?, ?, ?, ?)");
	groupQuery.addBindValue(groupId);
	groupQuery.addBindValue(sampleId);
	groupQuery.addBindValue("false");
	groupQuery.addBindValue(0);
	groupQuery.addBindValue("");
	if(!DatabaseManager::instance()->exec(&groupQuery))
		return false;

    LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::makeGroups(int resemblenceLevel)
{
	LOG("resemblence level: [%d]\n", resemblenceLevel);

	/* delete all group assignments */
	if(!DatabaseManager::instance()->clearTable("analyze_groupassignment"))
		return false;

	LOG("Successfully deleted all group assignments\n");

	/* get all samples */
	QSqlQuery samplesQuery(DatabaseManager::instance()->database());
	samplesQuery.prepare("SELECT * FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&samplesQuery))
		return false;

	/* for every sample try to assign it */
	while(samplesQuery.next()) {
		int sampleAssignId = samplesQuery.record().value("id").toInt();

		/* create QVector of loop hashes */
		QSqlQuery assignQuery(DatabaseManager::instance()->database());
		assignQuery.prepare("SELECT * FROM analyze_loopassignment WHERE sample_id = ?");
		assignQuery.addBindValue(sampleAssignId);
		if(!DatabaseManager::instance()->exec(&assignQuery))
			return false;

		QVector<QString> loopsHashes;
		while(assignQuery.next()) {
			int loopId = assignQuery.record().value("loop_id").toInt();

			QSqlQuery loopQuery(DatabaseManager::instance()->database());
			loopQuery.prepare("SELECT * FROM analyze_loop WHERE id = ?");
			loopQuery.addBindValue(loopId);
			if(!DatabaseManager::instance()->exec(&loopQuery))
				return false;

			loopQuery.next();
			loopsHashes.push_back(loopQuery.record().value("hash").toString());
		}

		/* find maching groups */
		QMap<int, int> machingGroups = findMatchingGroups(sampleAssignId, loopsHashes, resemblenceLevel);

		/* assign to maching groups */
		QMap<int, int>::iterator it;
		for(it = machingGroups.begin(); it != machingGroups.end(); ++it)
			assignToGroup(it.key(), sampleAssignId, it.value());
	}

	/* find and deactivate duplicate groups */
	activateUniqueGroups();

	/* count members in each group */
	countGroupMembers();

	LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::checkDuplicate(ExploitInfo *info)
{
    /* check sample */
	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare("SELECT * FROM analyze_sample WHERE name = ? AND extracted_from = ? AND file_size = ? AND shellcode_offset = ?");
	selectQuery.addBindValue(info->name());
	selectQuery.addBindValue(info->extractedFrom());
	selectQuery.addBindValue(info->size());
	selectQuery.addBindValue(info->codeOffset());
	if(!DatabaseManager::instance()->exec(&selectQuery))
        return false;

    LOG("SUCCESS\n\n");
	return selectQuery.next();
}

QMap<int, int> DatabaseOutput::findMatchingGroups(int id, QVector<QString> loopHashes, int resemblenceLevel)
{
	QMap<int, int> machingGroups;

	/* get all leaders */
	QSqlQuery leadersQuery(DatabaseManager::instance()->database());
	leadersQuery.prepare("SELECT * FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&leadersQuery))
		return QMap<int, int>();

	while(leadersQuery.next()) {
		int leaderId = leadersQuery.record().value("id").toInt();
		if(leaderId == id)
			continue;

		/* get all leader's loops */
		QSqlQuery assignQuery(DatabaseManager::instance()->database());
		assignQuery.prepare("SELECT * FROM analyze_loopassignment WHERE sample_id = ?");
		assignQuery.addBindValue(leaderId);
		if(!DatabaseManager::instance()->exec(&assignQuery))
			return QMap<int, int>();

		int resemblenceCounter = 0;
		int overallCounter = 0;
		while(assignQuery.next()) {
			int loopId = assignQuery.record().value("loop_id").toInt();

			QSqlQuery selectQuery(DatabaseManager::instance()->database());
			selectQuery.prepare("SELECT * FROM analyze_loop WHERE id = ?");
			selectQuery.addBindValue(loopId);
			if(!DatabaseManager::instance()->exec(&selectQuery))
				return QMap<int, int>();

			selectQuery.next();
			++overallCounter;
			if(loopHashes.contains(selectQuery.record().value("hash").toString()))
				++resemblenceCounter;
		}

		/* prevent floating point exception */
		if(overallCounter == 0) {
			LOG("sample doesn't have any loops, skipping, leader_id: [%d]\n", leaderId);
			continue;
		}

		/* compute resemlence rate */
		int resemblenceRate = (resemblenceCounter * 100) / overallCounter;
		LOG("resemblenceRate: [%d]\n", resemblenceRate);

		/* assign */
		if(resemblenceRate >= resemblenceLevel) {
			/* get all leader's group */
			QSqlQuery group_query(DatabaseManager::instance()->database());
			group_query.prepare("SELECT * FROM analyze_samplegroup WHERE leader_id = ?");
			group_query.addBindValue(leaderId);
			if(!DatabaseManager::instance()->exec(&group_query))
				return QMap<int, int>();

			group_query.next();
			int groupId = group_query.record().value("id").toInt();
			machingGroups.insert(groupId, resemblenceRate);
			LOG("resemblence rate: [%d] for group: [%d]\n", resemblenceRate, groupId);
		}
	}

	LOG("SUCCESS\n\n");
	return machingGroups;
}

void DatabaseOutput::assignToGroup(int groupId, int memberId, int resemblenceRate)
{
	/* get next samplegroup_id number */
	int assignId = DatabaseManager::instance()->sequenceValue("analyze_groupassignment_id_seq");

	/* check if assignment is unique */
	QSqlQuery check_query(DatabaseManager::instance()->database());
	check_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ? AND member_id = ?");
	check_query.addBindValue(groupId);
	check_query.addBindValue(memberId);
	if(!DatabaseManager::instance()->exec(&check_query))
		return;

	/* do not assign same sample to same group more than once */
	if(check_query.next())
		return;

	/* assign sample to group */
	QSqlQuery assign_query(DatabaseManager::instance()->database());
	assign_query.prepare("INSERT INTO analyze_groupassignment VALUES (?, ?, ?, ?)");
	assign_query.addBindValue(assignId);
	assign_query.addBindValue(groupId);
	assign_query.addBindValue(memberId);
	assign_query.addBindValue(resemblenceRate);
	if(!DatabaseManager::instance()->exec(&assign_query))
		return;

	LOG("SUCCESS\n\n");
}

void DatabaseOutput::activateUniqueGroups()
{
	/* get all groups */
	QSqlQuery select_query(DatabaseManager::instance()->database());
	select_query.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&select_query))
		return;

	QVector<int> activeGroups;

	/* for all groups */
	while(select_query.next()) {
		int groupId = select_query.record().value("id").toInt();

		/* check if is assigned to active groups in both ways */
		bool active = true;
		for(int i = 0; i < activeGroups.size(); ++i) {
			int activeId = activeGroups[i];
			active = !isDoubleConnected(groupId, activeId);
			if(!active)
				break;
		}

		if(active)
			activeGroups.push_back(groupId);
	}

	/* for all groups */
	QSqlQuery select2_query(DatabaseManager::instance()->database());
	select2_query.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&select2_query))
		return;

	while(select2_query.next()) {
		QString activation;
		int id = select2_query.record().value("id").toInt();
		if(activeGroups.contains(id))
			activation = "true";
		else
			activation = "false";

		/* update group */
		QSqlQuery update_query(DatabaseManager::instance()->database());
		update_query.prepare("UPDATE analyze_samplegroup SET active = ? WHERE id = ?");
		update_query.addBindValue(activation);
		update_query.addBindValue(id);
		if(!DatabaseManager::instance()->exec(&update_query))
			return;
	}

	LOG("SUCCESS\n\n");
}

bool DatabaseOutput::isDoubleConnected(int group1, int group2)
{
	/* get first leader */
	QSqlQuery leader1_query(DatabaseManager::instance()->database());
	leader1_query.prepare("SELECT leader_id FROM analyze_samplegroup WHERE id = ?");
	leader1_query.addBindValue(group1);
	if(!DatabaseManager::instance()->exec(&leader1_query))
		return false;

	leader1_query.next();
	int leader1 = leader1_query.record().value("leader_id").toInt();

	/* get second leader */
	QSqlQuery leader2_query(DatabaseManager::instance()->database());
	leader2_query.prepare("SELECT leader_id FROM analyze_samplegroup WHERE id = ?");
	leader2_query.addBindValue(group2);
	if(!DatabaseManager::instance()->exec(&leader2_query))
		return false;

	leader2_query.next();
	int leader2 = leader2_query.record().value("leader_id").toInt();

	/* check first way connection */
	bool connection1 = false;
	QSqlQuery select1_query(DatabaseManager::instance()->database());
	select1_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
	select1_query.addBindValue(group1);
	if(!DatabaseManager::instance()->exec(&select1_query))
		return false;

	while(select1_query.next()) {
		if(select1_query.record().value("member_id") == leader2) {
			connection1 = true;
			break;
		}
	}

	/* check second way connection */
	bool connection2 = false;
	QSqlQuery select2_query(DatabaseManager::instance()->database());
	select2_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
	select2_query.addBindValue(group2);
	if(!DatabaseManager::instance()->exec(&select2_query))
		return false;

	while(select2_query.next()) {
		if(select2_query.record().value("member_id") == leader1) {
			connection2 = true;
			break;
		}
	}

	return (connection1 && connection2);
}

void DatabaseOutput::countGroupMembers()
{
	/* get all groups */
	QSqlQuery select_query(DatabaseManager::instance()->database());
	select_query.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&select_query))
		return;

	/* for all groups */
	while(select_query.next()) {
		int groupId = select_query.record().value("id").toInt();

		/* get number of members in this group */
		QSqlQuery select2_query(DatabaseManager::instance()->database());
		select2_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
		select2_query.addBindValue(groupId);
		if(!DatabaseManager::instance()->exec(&select2_query))
			return;

		int membersNum = select2_query.size() + 1;	// +1 because we count also the leader
		LOG("groupId: [%d], membersNum: [%d]\n", groupId, membersNum);

		/* update number of members for this group */
		QSqlQuery update_query(DatabaseManager::instance()->database());
		update_query.prepare("UPDATE analyze_samplegroup SET members_num = ? WHERE id = ?");
		update_query.addBindValue(membersNum);
		update_query.addBindValue(groupId);
		if(!DatabaseManager::instance()->exec(&update_query))
			return;
	}

	LOG("SUCCESS\n\n");
}
