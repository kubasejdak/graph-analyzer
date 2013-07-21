/*
 * Filename		: SampleExportOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 19-07-2013
 */

#include "SampleExportOutput.h"

#include <string>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>

#include <core/ExploitSample.h>
#include <core/ExploitInfo.h>
#include <utils/SystemLogger.h>
#include <utils/UniqueNameResolver.h>
#include <tasks/export/modules/output/IOutput.h>

using namespace std;
using namespace Export;

#define	EXPORT_FILE	string("graph-analyzer_exported_samples")

SampleExportOutput::SampleExportOutput()
{
	m_name = "file";
	m_description = "Exports samples as binary files and generates txt list about them.";
}

bool SampleExportOutput::exportOutput(ExploitSampleHandle sample, string exportDir)
{
	// check if sample is from PCAP file, if not return
	if(sample->info()->fileType() != "application/vnd.tcpdump.pcap") {
		LOG("sample [%s] is not a PCAP file [%s], skipping\n", sample->info()->name().c_str(), sample->info()->fileType().c_str());
		LOG("SUCCESS\n\n");
		return true;
	}

	string outputDir = exportDir;
	outputDir = Toolbox::removeSlash(outputDir);

	// check if output directory exists
	if(boost::filesystem::exists(outputDir) == false) {
		if(boost::filesystem::create_directories(outputDir) == false) {
			LOG_ERROR("creating export directory failed: [%s]\n", outputDir.c_str());
			LOG_ERROR("FAILURE\n\n");
			return false;
		}
	}

	// check if output file exists
	string outputFile = outputDir + "/" + EXPORT_FILE;
	ofstream file;
	if(boost::filesystem::exists(outputFile) == false) {
		LOG("export file [%s] doesn't exist, creating and opening in truncate mode\n", outputFile.c_str());
		file.open(outputFile);
	}
	else {
		LOG("export file [%s] exists, opening in append mode\n", outputFile.c_str());
		file.open(outputFile, ios_base::out | ios_base::app);
	}

	// copy code into separate file
	string exportedSampleFilename = outputDir + "/" + sample->info()->name();
	UniqueNameResolver nameResolver;
	exportedSampleFilename = nameResolver.resolve(exportedSampleFilename);

	ofstream sampleFile;
	sampleFile.open(exportedSampleFilename);
	if(sampleFile.is_open() == false) {
		LOG_ERROR("cannot create sample export file: [%s]\n", exportedSampleFilename.c_str());
		LOG_ERROR("FAILURE\n\n");
		return false;
	}
	LOG("created sample export file: [%s]\n", exportedSampleFilename.c_str());

	sampleFile.write(reinterpret_cast<const char *>(sample->code()), sample->info()->fileSize());
	sampleFile.close();

	// prepare line that will be inserted to export file
	stringstream exportLine;

	// it is assumed that this is a PCAP file and it has proper name (just like this from tcpflow)
	exportLine << sample->info()->id() << "\t";
	exportLine << sample->info()->name() << "\t";
	exportLine << sample->info()->captureDate().toString("yyyyMMdd").toStdString() << "\t";
	exportLine << exportedSampleFilename << "\n";

	// save line to file
	LOG("export line: [%s]\n", exportLine.str().c_str());
	file << exportLine.str();
	file.close();

	LOG("SUCCESS\n\n");
	return true;
}
