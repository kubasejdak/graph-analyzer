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

bool DatabaseOutput::generateOutput(ShellcodeSample *sample)
{
    if(!sample->info()->isShellcodePresent() && SKIP_NONEXPLOIT_OUTPUT) {
        LOG("no exploit found, returning\n");
        LOG("SUCCESS\n\n");
        return true;
    }

    ShellcodeInfo *info = sample->info();

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
    QSqlQuery seq_query(DatabaseManager::instance()->database());
    seq_query.prepare("SELECT nextval('analyze_sample_id_seq')");
    if(!DatabaseManager::instance()->exec(&seq_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }
    seq_query.next();
	int sampleId = seq_query.record().value("nextval").toInt();

    /* general sample data */
    QSqlQuery sample_query(DatabaseManager::instance()->database());
	sample_query.prepare("INSERT INTO analyze_sample VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
	sample_query.bindValue(0, sampleId);
    sample_query.bindValue(1, info->name());
    sample_query.bindValue(2, info->extractedFrom());
    sample_query.bindValue(3, info->graphName());
    sample_query.bindValue(4, info->fileType());
    sample_query.bindValue(5, QString().setNum(info->size()));
    sample_query.bindValue(6, QString().setNum(info->codeOffset()));
	sample_query.bindValue(7, "");
    if(!DatabaseManager::instance()->exec(&sample_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    /* analyze modules specific data */
    QMultiMap<QString, QMap<QString, QString> *> *t = sample->info()->traits();
    QMultiMap<QString, QMap<QString, QString> *>::iterator it;
    QSqlQuery mod_query(DatabaseManager::instance()->database());
    QString table;
    for(it = t->begin(); it != t->end(); ++it) {
        table = ANA_TABLE_PREFIX;
        table += it.key();

		traitQuery(&mod_query, table, it.value(), sampleId);
        if(!DatabaseManager::instance()->exec(&mod_query)) {
            SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
            LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
            LOG_ERROR("FAILURE\n\n");
            return false;
        }
    }

	/* get next samplegroup_id number */
	QSqlQuery seq2_query(DatabaseManager::instance()->database());
	seq2_query.prepare("SELECT nextval('analyze_samplegroup_id_seq')");
	if(!DatabaseManager::instance()->exec(&seq2_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	seq2_query.next();
	int groupId = seq2_query.record().value("nextval").toInt();

	/* general samplegroup data */
	QSqlQuery group_query(DatabaseManager::instance()->database());
	group_query.prepare("INSERT INTO analyze_samplegroup VALUES (?, ?, ?, ?, ?)");
	group_query.bindValue(0, groupId);
	group_query.bindValue(1, sampleId);
	group_query.bindValue(2, "false");
	group_query.bindValue(3, 0);
	group_query.bindValue(4, "");
	if(!DatabaseManager::instance()->exec(&group_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

    LOG("SUCCESS\n\n");
	return true;
}

bool DatabaseOutput::makeGroups(int resemblenceLevel)
{
	LOG("resemblence level: [%d]\n", resemblenceLevel);

	/* delete all group assignments */
	QSqlQuery delete_query(DatabaseManager::instance()->database());
	delete_query.prepare("DELETE FROM analyze_groupassignment");
	if(!DatabaseManager::instance()->exec(&delete_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	LOG("Successfully deleted all group assignments\n");

	/* get all samples */
	QSqlQuery samples_query(DatabaseManager::instance()->database());
	samples_query.prepare("SELECT * FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&samples_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}

	/* for every sample try to assign it */
	while(samples_query.next()) {
		int sampleAssignId = samples_query.record().value("id").toInt();

		/* create QVector of samples loop hashes */
		QSqlQuery loops_query(DatabaseManager::instance()->database());
		loops_query.prepare("SELECT hash FROM analyze_loop WHERE sample_id = ?");
		loops_query.bindValue(0, sampleAssignId);
		if(!DatabaseManager::instance()->exec(&loops_query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
		QVector<QString> loopsHashes;
		while(loops_query.next())
			loopsHashes.push_back(loops_query.record().value("hash").toString());

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

void DatabaseOutput::traitQuery(QSqlQuery *query, QString table, QMap<QString, QString> *v, int sample_id)
{
    QMap<QString, QString>::iterator it;

	/* prepare query */
    QString str_q = QString("INSERT INTO %1 VALUES (DEFAULT, ").arg(table);
    for(int i = 0; i < v->size() + 1; ++i) { // +1 because sample_id
		str_q += "?";
		if(i < v->size())
			str_q += ", ";
	}
	str_q += ")";

	/* bind values */
    query->prepare(str_q);
    query->bindValue(0, sample_id);
	int i = 1;
	for(it = v->begin(); it != v->end(); ++it) {
        query->bindValue(i, it.value());
		++i;
	}
}

bool DatabaseOutput::checkDuplicate(ShellcodeInfo *info)
{
    /* check sample */
    QSqlQuery select_query(DatabaseManager::instance()->database());
    select_query.prepare("SELECT * FROM analyze_sample WHERE name = ? AND extracted_from = ? AND file_size = ? AND shellcode_offset = ?");
    select_query.addBindValue(info->name());
    select_query.addBindValue(info->extractedFrom());
    select_query.addBindValue(info->size());
    select_query.addBindValue(info->codeOffset());
    if(!DatabaseManager::instance()->exec(&select_query)) {
        SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
        LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    LOG("SUCCESS\n\n");
    return select_query.next();
}

QMap<int, int> DatabaseOutput::findMatchingGroups(int id, QVector<QString> loopHashes, int resemblenceLevel)
{
	QMap<int, int> machingGroups;

	/* get all leaders */
	QSqlQuery leaders_query(DatabaseManager::instance()->database());
	leaders_query.prepare("SELECT id FROM analyze_sample");
	if(!DatabaseManager::instance()->exec(&leaders_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return QMap<int, int>();
	}

	while(leaders_query.next()) {
		int leaderId = leaders_query.record().value("id").toInt();
		if(leaderId == id)
			continue;

		/* get all leader's loops */
		QSqlQuery select_query(DatabaseManager::instance()->database());
		select_query.prepare("SELECT * FROM analyze_loop WHERE sample_id = ?");
		select_query.addBindValue(leaderId);
		if(!DatabaseManager::instance()->exec(&select_query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
			LOG_ERROR("FAILURE\n\n");
			return QMap<int, int>();
		}

		int resemblenceCounter = 0;
		int overallCounter = 0;
		while(select_query.next()) {
			++overallCounter;
			if(loopHashes.contains(select_query.record().value("hash").toString()))
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
			group_query.prepare("SELECT id FROM analyze_samplegroup WHERE leader_id = ?");
			group_query.addBindValue(leaderId);
			if(!DatabaseManager::instance()->exec(&group_query)) {
				SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
				LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
				LOG_ERROR("FAILURE\n\n");
				return QMap<int, int>();
			}
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
	QSqlQuery seq_query(DatabaseManager::instance()->database());
	seq_query.prepare("SELECT nextval('analyze_groupassignment_id_seq')");
	if(!DatabaseManager::instance()->exec(&seq_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return;
	}

	seq_query.next();
	int assignId = seq_query.record().value("nextval").toInt();

	/* check if assignment is unique */
	QSqlQuery check_query(DatabaseManager::instance()->database());
	check_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ? AND member_id = ?");
	check_query.addBindValue(groupId);
	check_query.addBindValue(memberId);
	if(!DatabaseManager::instance()->exec(&check_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return;
	}

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
	if(!DatabaseManager::instance()->exec(&assign_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return;
	}

	LOG("SUCCESS\n\n");
}

void DatabaseOutput::activateUniqueGroups()
{
	/* get all groups */
	QSqlQuery select_query(DatabaseManager::instance()->database());
	select_query.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&select_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return;
	}

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
	if(!DatabaseManager::instance()->exec(&select2_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return;
	}
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
		update_query.addBindValue(activation.toStdString().c_str());
		update_query.addBindValue(id);
		if(!DatabaseManager::instance()->exec(&update_query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
			LOG_ERROR("FAILURE\n\n");
			return;
		}
	}

	LOG("SUCCESS\n\n");
}

bool DatabaseOutput::isDoubleConnected(int group1, int group2)
{
	/* get first leader */
	QSqlQuery leader1_query(DatabaseManager::instance()->database());
	leader1_query.prepare("SELECT leader_id FROM analyze_samplegroup WHERE id = ?");
	leader1_query.addBindValue(group1);
	if(!DatabaseManager::instance()->exec(&leader1_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	leader1_query.next();
	int leader1 = leader1_query.record().value("leader_id").toInt();

	/* get second leader */
	QSqlQuery leader2_query(DatabaseManager::instance()->database());
	leader2_query.prepare("SELECT leader_id FROM analyze_samplegroup WHERE id = ?");
	leader2_query.addBindValue(group2);
	if(!DatabaseManager::instance()->exec(&leader2_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	leader2_query.next();
	int leader2 = leader2_query.record().value("leader_id").toInt();

	/* check first way connection */
	bool connection1 = false;
	QSqlQuery select1_query(DatabaseManager::instance()->database());
	select1_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
	select1_query.addBindValue(group1);
	if(!DatabaseManager::instance()->exec(&select1_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
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
	if(!DatabaseManager::instance()->exec(&select2_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	while(select2_query.next()) {
		if(select2_query.record().value("member_id") == leader1) {
			connection2 = true;
			break;
		}
	}

	LOG("SUCCESS\n\n");
	return (connection1 && connection2);
}

void DatabaseOutput::countGroupMembers()
{
	/* get all groups */
	QSqlQuery select_query(DatabaseManager::instance()->database());
	select_query.prepare("SELECT * FROM analyze_samplegroup");
	if(!DatabaseManager::instance()->exec(&select_query)) {
		SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
		LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
		LOG_ERROR("FAILURE\n\n");
		return;
	}

	/* for all groups */
	while(select_query.next()) {
		int groupId = select_query.record().value("id").toInt();

		/* get number of members in this group */
		QSqlQuery select2_query(DatabaseManager::instance()->database());
		select2_query.prepare("SELECT * FROM analyze_groupassignment WHERE group_id = ?");
		select2_query.addBindValue(groupId);
		if(!DatabaseManager::instance()->exec(&select2_query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
			LOG_ERROR("FAILURE\n\n");
			return;
		}
		int membersNum = select2_query.size() + 1;	// +1 because we count also the leader
		LOG("groupId: [%d], membersNum: [%d]\n", groupId, membersNum);

		/* update number of members for this group */
		QSqlQuery update_query(DatabaseManager::instance()->database());
		update_query.prepare("UPDATE analyze_samplegroup SET members_num = ? WHERE id = ?");
		update_query.addBindValue(membersNum);
		update_query.addBindValue(groupId);
		if(!DatabaseManager::instance()->exec(&update_query)) {
			SystemLogger::instance()->setError(DatabaseManager::instance()->lastError());
			LOG_ERROR("%s\n", DatabaseManager::instance()->lastError().toStdString().c_str());
			LOG_ERROR("FAILURE\n\n");
			return;
		}
	}

	LOG("SUCCESS\n\n");
}
