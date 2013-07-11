/*
 * Filename		: GroupManager.h
 * Author		: Kuba Sejdak
 * Created on	: 09-12-2012
 */

#ifndef GROUPMANAGER_H_
#define GROUPMANAGER_H_

#include <vector>

#include <core/ExploitSample.h>

typedef std::vector<SampleVector> GroupsVector;

class GroupManager {
public:
	GroupManager() {}
	virtual ~GroupManager() {}


	int createGroup();
	void add(int groupId, ExploitSampleHandle sample);

	int find(ExploitSampleHandle sample);
	ExploitSampleHandle leader(int groupId);

	int count();
	int count(int groupId);

private:
	GroupsVector m_groups;
};

#endif /* GROUPMANAGER_H_ */
