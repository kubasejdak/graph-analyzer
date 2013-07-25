/*
 * Filename		: IndexFileInput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 25-08-2013
 */

#include "IndexFileInput.h"

#include <string>
#include <sstream>
#include <fstream>

#include <tasks/analyze/modules/input/IInput.h>
#include <tasks/analyze/modules/ModulesManager.h>
#include <core/ExploitSample.h>
#include <utils/SystemLogger.h>

using namespace std;
using namespace Analyze;

IndexFileInput::IndexFileInput()
{
	m_name = "IndexFileInput";
	m_type = "graph-analyzer/index-file";
	m_description = "Loads exploits from index file created by export task.";
}

bool IndexFileInput::loadInput(std::string filename, SampleList *samples)
{
	ifstream indexFile;
	indexFile.open(filename);

	// read first line (file type line)
	string firstLine;
	getline(indexFile, firstLine);

	LOG("reading index file\n");
	while(true) {
		// read file line
		string line;
		getline(indexFile, line);

		// check if EOF
		if(indexFile.eof())
			break;

		// read all columns
		stringstream lineStream(line);
		string id, name, captureDate, sampleFile;
		lineStream >> id >> name >> captureDate >> sampleFile;
		LOG("id: [%s], name: [%s], captureDate: [%s], sampleFile: [%s]\n", id.c_str(), name.c_str(), captureDate.c_str(), sampleFile.c_str());

		// load file using BinaryInput module
		SampleList q;
		bool ret = (*ModulesManager::instance()->input())["BinaryInput"]->loadInput(sampleFile, &q);
		if(ret == false) {
			LOG_ERROR("failed to load exported file: [%s]\n", sampleFile.c_str());
			LOG_ERROR("FAILURE\n\n");
			return false;
		}

		if(q.empty())
			LOG("WARNING: export file didn't contain exploit\n");

		while(q.empty() == false) {
			ExploitSampleHandle sample = q.front();
			q.pop_front();

			sample->info()->setCaptureDate(captureDate);
			samples->insert(samples->end(), sample);
		}
	}

	LOG("SUCCESS\n\n");
	return true;
}
