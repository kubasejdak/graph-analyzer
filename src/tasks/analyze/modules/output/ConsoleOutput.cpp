/*
 * Filename		: ConsoleOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#include "ConsoleOutput.h"

#include <iostream>
#include <iomanip>

#include <core/ExploitSample.h>
#include <tasks/analyze/modules/output/IOutput.h>

using namespace std;
using namespace Analyze;

ConsoleOutput::ConsoleOutput()
{
	m_name = "console";
    m_description = "Prints info about samples in console.";
}

bool ConsoleOutput::exportOutput(ExploitSampleHandle sample, int taskId, bool override)
{	
	ExploitInfoHandle info = sample->info();

	cout << "\t name : " << info->name() << endl;
	cout << "\t extracted from: " << info->extractedFrom() << endl;
	cout << "\t graph name: " << info->graphName() << endl;
	cout << "\t capture date: " << info->captureDate().toString("yyyy-MM-dd").toStdString() << endl;
	cout << "\t size : " << dec << info->size() << endl;
	cout << "\t file type : " << info->fileType() << endl;
	cout << "\t file size : " << dec << info->fileSize() << endl;
	cout << "\t exploit present : " << (info->isExploitPresent() ? "yes" : "no") << endl;
	cout << "\t exploit offset : 0x" << hex << info->codeOffset() << endl;
    cout << "\t task: " << dec << taskId << endl;

	TraitMap::iterator it;
	TraitEntry::iterator it2;
	for(it = info->traits()->begin(); it != info->traits()->end(); ++it) {
		cout << "\t * " << it.key() << " *" << "\t";
		for(it2 = it.value()->begin(); it2 != it.value()->end(); ++it2)
			cout << it2.key() << " : " << it2.value() << ", ";
		cout << endl << endl;
	}

	return true;
}
