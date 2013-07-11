/*
 * Filename		: GroupManager.cpp
 * Author		: Kuba Sejdak
 * Created on	: 09-12-2012
 */

#include "GroupManager.h"

#include <vector>

#include <core/ExploitSample.h>

using namespace std;

int GroupManager::createGroup()
{
	m_groups.resize(m_groups.size() + 1);
	return m_groups.size() - 1;
}

void GroupManager::add(int groupId, ExploitSampleHandle sample)
{
	m_groups[groupId].push_back(sample);
}

int GroupManager::find(ExploitSampleHandle sample)
{
	int id = -1;

	for(unsigned int i = 0; i < m_groups.size(); ++i) {
		for(ExploitSampleHandle s : m_groups[i]) {
			if(s->info()->name() == sample->info()->name() && s->info()->extractedFrom() == sample->info()->extractedFrom()) {
				id = i;
				goto exit;
			}
		}
	}

exit:
	return id;
}

ExploitSampleHandle GroupManager::leader(int groupId)
{
	return m_groups[groupId][0];
}

int GroupManager::count()
{
	return m_groups.size();
}

int GroupManager::count(int groupId)
{
	return m_groups[groupId].size();
}
