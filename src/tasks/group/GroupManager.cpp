/*
 * Filename		: GroupManager.cpp
 * Author		: Kuba Sejdak
 * Created on	: 09-12-2012
 */

#include "GroupManager.h"
#include <core/ExploitSample.h>
#include <utils/DatabaseManager.h>
#include <utils/SystemLogger.h>

#include <QtSql>

void GroupManager::processOneSampleGroup(int sampleId, int groupId, int resemblenceLevel)
{
	LOG("resemblence level: [%d]\n", resemblenceLevel);

	/* ====== PART 1: assign sample to all existing groups ====== */

	/* find maching groups */
	QMap<int, int> machingGroups = findMatchingGroups(sampleId, resemblenceLevel);

	/* assign to maching groups */
	QMap<int, int>::iterator itGroup;
	for(itGroup = machingGroups.begin(); itGroup != machingGroups.end(); ++itGroup)
		assignToGroup(itGroup.key(), sampleId, itGroup.value());

	/* ====== PART 2: assign all samples to new sample group ====== */ /* <--------------- OK */

	/* find maching samples */
	QMap<int, int> machingSamples = findMatchingSamples(sampleId, resemblenceLevel);

	/* assign to maching samples */
	LOG("assigning samples to one group, leaderId: [%d]\n", sampleId);
	QMap<int, int>::iterator itSample;
	for(itSample = machingSamples.begin(); itSample != machingSamples.end(); ++itSample)
		assignToGroup(groupId, itSample.key(), itSample.value());
}

void GroupManager::processAllSampleGroups(int resemblenceLevel)
{
	/* clear current group assignments */
	DatabaseManager::instance()->clearTable("analyze_groupassignment");

	/* get all samples */
	QSqlQuery samplesQuery(DatabaseManager::instance()->database());
	samplesQuery.prepare("SELECT * FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&samplesQuery))
		return;

	/* for every sample */
	while(samplesQuery.next()) {
		int sampleId = samplesQuery.record().value("id").toInt();

		/* process samples group */
		QSqlQuery idQuery(DatabaseManager::instance()->database());
		idQuery.prepare("SELECT * FROM analyze_samplegroup WHERE leader_id = ?");
		idQuery.addBindValue(sampleId);
		if(!DatabaseManager::instance()->exec(&idQuery))
			return;

		idQuery.next();
		int groupId = idQuery.record().value("id").toInt();
		processOneSampleGroup(sampleId, groupId, resemblenceLevel);
	}
}

QMap<int, int> GroupManager::findMatchingGroups(int sampleId, int resemblenceLevel)
{
	QMap<int, int> machingGroups;

	/* get all leaders */
	QSqlQuery leadersQuery(DatabaseManager::instance()->database());
	leadersQuery.prepare("SELECT * FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&leadersQuery))
		return QMap<int, int>();

	while(leadersQuery.next()) {
		int leaderId = leadersQuery.record().value("id").toInt();
		if(leaderId == sampleId)
			continue;

		int resemblenceRate = resemblenceToGroup(sampleId, leaderId);

		/* add */
		if(resemblenceRate >= resemblenceLevel) {
			/* get group id */
			QSqlQuery groupQuery(DatabaseManager::instance()->database());
			groupQuery.prepare("SELECT * FROM analyze_samplegroup WHERE leader_id = ?");
			groupQuery.addBindValue(leaderId);
			if(!DatabaseManager::instance()->exec(&groupQuery))
				return QMap<int, int>();

			groupQuery.next();
			int groupId = groupQuery.record().value("id").toInt();
			machingGroups[groupId] = resemblenceRate;
			LOG("resemblence rate: [%d] for group: [%d]\n", resemblenceRate, groupId);
		}
	}

	LOG("SUCCESS\n\n");
	return machingGroups;
}

QMap<int, int> GroupManager::findMatchingSamples(int leaderId, int resemblenceLevel)
{
	QMap<int, int> machingSamples;

	/* get all samples */
	QSqlQuery samplesQuery(DatabaseManager::instance()->database());
	samplesQuery.prepare("SELECT * FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&samplesQuery))
		return QMap<int, int>();

	while(samplesQuery.next()) {
		int sampleId = samplesQuery.record().value("id").toInt();
		if(sampleId == leaderId)
			continue;

		/* get resemblence */
		int resemblenceRate = resemblenceToGroup(sampleId, leaderId);

		/* add */
		if(resemblenceRate >= resemblenceLevel) {
			machingSamples[sampleId] = resemblenceRate;
			LOG("resemblence rate: [%d] for sample: [%d]\n", resemblenceRate, sampleId);
		}
	}

	LOG("SUCCESS\n\n");
	return machingSamples;
}

int GroupManager::resemblenceToGroup(int sampleId, int leaderId)
{
	/* get all sample loop hashes */
	QVector<QString> sampleHashes = loopHashes(sampleId);

	/* get all leader's loops */
	QVector<QString> leaderHashes = loopHashes(leaderId);

	/* compare samples and leaders loop hashes */
	int resemblenceCounter = 0;
	int overallCounter = 0;
	for(int i = 0; i < leaderHashes.size(); ++i) {
		++overallCounter;
		if(sampleHashes.contains(leaderHashes[i]))
			++resemblenceCounter;
	}

	/* prevent floating point exception */
	if(overallCounter == 0) {
		LOG("leader doesn't have any loops, skipping, leader_id: [%d]\n", leaderId);
		return 0;
	}

	/* compute resemlence rate */
	int resemblenceRate = (resemblenceCounter * 100) / overallCounter;
	LOG("resemblenceRate: [%d]\n", resemblenceRate);

	return resemblenceRate;
}

void GroupManager::assignToGroup(int groupId, int memberId, int resemblenceRate)
{
	LOG("assigning member: [%d] to group: [%d]\n", memberId, groupId);

	/* get next samplegroup_id number */
	int assignId = DatabaseManager::instance()->sequenceValue("analyze_groupassignment_id_seq");

	/* check if assignment is unique */
	QSqlQuery checkQuery(DatabaseManager::instance()->database());
	checkQuery.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ? AND member_id = ?");
	checkQuery.addBindValue(groupId);
	checkQuery.addBindValue(memberId);
	if(!DatabaseManager::instance()->exec(&checkQuery))
		return;

	/* do not assign same sample to same group more than once */
	if(checkQuery.next())
		return;

	/* assign sample to group */
	QSqlQuery assignQuery(DatabaseManager::instance()->database());
	assignQuery.prepare("INSERT INTO analyze_groupassignment VALUES (?, ?, ?, ?)");
	assignQuery.addBindValue(assignId);
	assignQuery.addBindValue(groupId);
	assignQuery.addBindValue(memberId);
	assignQuery.addBindValue(resemblenceRate);
	if(!DatabaseManager::instance()->exec(&assignQuery))
		return;

	LOG("SUCCESS\n\n");
}

QVector<QString> GroupManager::loopHashes(int sampleId)
{
	/* get all sample loop hashes */
	QSqlQuery assignQuery(DatabaseManager::instance()->database());
	assignQuery.prepare("SELECT * FROM analyze_loopassignment WHERE sample_id = ?");
	assignQuery.addBindValue(sampleId);
	if(!DatabaseManager::instance()->exec(&assignQuery))
		return QVector<QString>();

	QVector<QString> hashes;
	while(assignQuery.next()) {
		int loopId = assignQuery.record().value("loop_id").toInt();

		QSqlQuery loopQuery(DatabaseManager::instance()->database());
		loopQuery.prepare("SELECT * FROM analyze_loop WHERE id = ?");
		loopQuery.addBindValue(loopId);
		if(!DatabaseManager::instance()->exec(&loopQuery))
			return QVector<QString>();

		loopQuery.next();
		hashes.push_back(loopQuery.record().value("hash").toString());
	}

	return hashes;
}

void GroupManager::countGroupsMembers()
{
	/* get all groups */
	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&selectQuery))
		return;

	/* for all groups */
	while(selectQuery.next()) {
		int groupId = selectQuery.record().value("id").toInt();

		/* get number of members in this group */
		QSqlQuery select2Query(DatabaseManager::instance()->database());
		select2Query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
		select2Query.addBindValue(groupId);
		if(!DatabaseManager::instance()->exec(&select2Query))
			return;

		int membersNum = select2Query.size() + 1;	// +1 because we count also the leader
		LOG("groupId: [%d], membersNum: [%d]\n", groupId, membersNum);

		/* update number of members for this group */
		QSqlQuery updateQuery(DatabaseManager::instance()->database());
		updateQuery.prepare("UPDATE analyze_samplegroup SET members_num = ? WHERE id = ?");
		updateQuery.addBindValue(membersNum);
		updateQuery.addBindValue(groupId);
		if(!DatabaseManager::instance()->exec(&updateQuery))
			return;
	}

	LOG("SUCCESS\n\n");
}

void GroupManager::activateUniqueGroups()
{
	/* get all groups */
	QSqlQuery selectQuery(DatabaseManager::instance()->database());
	selectQuery.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&selectQuery))
		return;

	QVector<int> activeGroups;

	/* for all groups */
	while(selectQuery.next()) {
		int groupId = selectQuery.record().value("id").toInt();

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
	QSqlQuery select2Query(DatabaseManager::instance()->database());
	select2Query.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&select2Query))
		return;

	while(select2Query.next()) {
		QString activation;
		int id = select2Query.record().value("id").toInt();
		if(activeGroups.contains(id))
			activation = "true";
		else
			activation = "false";

		/* update group */
		QSqlQuery updateQuery(DatabaseManager::instance()->database());
		updateQuery.prepare("UPDATE analyze_samplegroup SET active = ? WHERE id = ?");
		updateQuery.addBindValue(activation);
		updateQuery.addBindValue(id);
		if(!DatabaseManager::instance()->exec(&updateQuery))
			return;
	}

	LOG("SUCCESS\n\n");
}

bool GroupManager::isDoubleConnected(int group1Id, int group2Id)
{
	/* get first leader */
	QSqlQuery leader1Query(DatabaseManager::instance()->database());
	leader1Query.prepare("SELECT leader_id FROM analyze_samplegroup WHERE id = ?");
	leader1Query.addBindValue(group1Id);
	if(!DatabaseManager::instance()->exec(&leader1Query))
		return false;

	leader1Query.next();
	int leader1Id = leader1Query.record().value("leader_id").toInt();

	/* get second leader */
	QSqlQuery leader2Query(DatabaseManager::instance()->database());
	leader2Query.prepare("SELECT leader_id FROM analyze_samplegroup WHERE id = ?");
	leader2Query.addBindValue(group2Id);
	if(!DatabaseManager::instance()->exec(&leader2Query))
		return false;

	leader2Query.next();
	int leader2Id = leader2Query.record().value("leader_id").toInt();

	/* check first way connection */
	bool connection1 = false;
	QSqlQuery select1Query(DatabaseManager::instance()->database());
	select1Query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
	select1Query.addBindValue(group1Id);
	if(!DatabaseManager::instance()->exec(&select1Query))
		return false;

	while(select1Query.next()) {
		if(select1Query.record().value("member_id") == leader2Id) {
			connection1 = true;
			break;
		}
	}

	/* check second way connection */
	bool connection2 = false;
	QSqlQuery select2Query(DatabaseManager::instance()->database());
	select2Query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
	select2Query.addBindValue(group2Id);
	if(!DatabaseManager::instance()->exec(&select2Query))
		return false;

	while(select2Query.next()) {
		if(select2Query.record().value("member_id") == leader1Id) {
			connection2 = true;
			break;
		}
	}

	return (connection1 && connection2);
}
