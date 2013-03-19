/*
 * Filename		: GroupManager.h
 * Author		: Kuba Sejdak
 * Created on	: 09-12-2012
 */

#ifndef GROUPMANAGER_H_
#define GROUPMANAGER_H_

#include <QVector>
#include <QString>
#include <QMap>

class GroupManager {
public:
	GroupManager() {}
	virtual ~GroupManager() {}

	void processOneSampleGroup(int sampleId, int groupId, int resemblenceLevel);
	void processAllSampleGroups(int resemblenceLevel);
	void countGroupsMembers();
	void activateUniqueGroups();

private:
	QMap<int, int> findMatchingGroups(int sampleId, int resemblenceLevel);
	QMap<int, int> findMatchingSamples(int leaderId, int resemblenceLevel);

	int resemblenceToGroup(int sampleId, int leaderId);
	void assignToGroup(int groupId, int memberId, int resemblenceRate);
	QVector<QString> loopHashes(int sampleId);

	bool isDoubleConnected(int group1Id, int group2Id);
};

#endif /* GROUPMANAGER_H_ */
