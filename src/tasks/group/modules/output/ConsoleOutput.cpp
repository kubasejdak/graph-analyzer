/*
 * Filename		: ConsoleOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2013
 */

#include "ConsoleOutput.h"

#include <iostream>
#include <iomanip>

#include <tasks/group/modules/output/IOutput.h>
#include <core/ExploitGroup.h>

using namespace std;
using namespace Group;

ConsoleOutput::ConsoleOutput()
{
	m_name = "console";
    m_description = "Prints info about groups in console.";
}

bool ConsoleOutput::exportOutput(ExploitGroupHandle group, int taskId, bool override)
{
	cout << "\t size : " << dec << group->size() << endl;
	cout << "\t task: " << dec << taskId << endl;
	cout << "\tSamples in group:" << endl;
	for(int i = 0; i < group->size(); ++i)
		cout << "\t\t [" << i << "] " << "name: [" << group->get(i)->info()->name() << "], extracted from: [" << group->get(i)->info()->extractedFrom() << "]" << endl;

	return true;
}
