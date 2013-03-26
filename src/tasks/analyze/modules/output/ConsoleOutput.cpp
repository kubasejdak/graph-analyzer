/*
 * Filename		: ConsoleOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#include "ConsoleOutput.h"
#include <core/ExploitSample.h>

#include <iostream>
#include <iomanip>
using namespace std;

ConsoleOutput::ConsoleOutput()
{
	m_name = "console";
    m_description = "Prints info about samples in console.";
}

bool ConsoleOutput::exportOutput(ExploitSample *sample)
{	
	ExploitInfo *info = sample->info();

	cout << "\t name : " << info->name().toStdString() << endl;
	cout << "\t extracted from: " << info->extractedFrom().toStdString() << endl;
	cout << "\t graph name: " << info->graphName().toStdString() << endl;
	cout << "\t capture date: " << info->captureDate().toString("yyyy-MM-dd").toStdString() << endl;
	cout << "\t size : " << dec << info->size() << endl;
	cout << "\t file type : " << info->fileType().toStdString() << endl;
	cout << "\t file size : " << dec << info->fileSize() << endl;
	cout << "\t exploit present : " << (info->isExploitPresent() ? "yes" : "no") << endl;
	cout << "\t exploit offset : 0x" << hex << info->codeOffset() << endl;

	TraitsMap::iterator it;
	TraitsEntry::iterator it2;
	for(it = info->traits()->begin(); it != info->traits()->end(); ++it) {
		cout << "\t * " << it.key().toStdString() << " *" << "\t";
		for(it2 = it.value()->begin(); it2 != it.value()->end(); ++it2)
			cout << it2.key().toStdString() << " : " << it2.value().toStdString() << ", ";
		cout << endl << endl;
	}

	return true;
}
