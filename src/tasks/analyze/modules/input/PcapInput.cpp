/*
 * Filename		: PcapInput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "PcapInput.h"

#include <string>
#include <sstream>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

#include <tasks/analyze/modules/input/IInput.h>
#include <core/ExploitSample.h>
#include <core/Options.h>
#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>

using namespace std;

PcapInput::PcapInput()
{
    m_name = "PcapInput";
	m_type = "application/vnd.tcpdump.pcap";
	m_description = "Loads exploit from pcap files.";
}

bool PcapInput::loadInput(string filename, SampleList *samples)
{
    int success;
	string tmpPcapDir = Options::instance()->tmpPcapDir;

	// move to /tmp/pcap_tmp
    QDir pcapDir;
	if(!QDir(tmpPcapDir.c_str()).exists()) {
		success = pcapDir.mkdir(tmpPcapDir.c_str());
        if(!success) {
			LOG_ERROR("cannot create [%s] directory\n", tmpPcapDir.c_str());
            LOG_ERROR("FAILURE\n\n");
            return false;
		}
	}

	success = QDir::setCurrent(tmpPcapDir.c_str());
    if(!success) {
		LOG_ERROR("cannot change directory to [%s]\n", tmpPcapDir.c_str());
        Toolbox::removeDirectory(tmpPcapDir);
		LOG_ERROR("FAILURE\n\n");
        return false;
	}

	// create flow files
	QFileInfo f(filename.c_str());
	stringstream ss;
	ss << "tcpflow -r \"" << f.absoluteFilePath().toStdString() << "\" " << Options::instance()->tcpflowParams;
	string tcpflowCmd = ss.str();

	int ret = system(tcpflowCmd.c_str());
    if(ret != 0) {
        SystemLogger::instance()->setError("external program 'tcpflow' failed");
        LOG_ERROR("tcpflow cmd failed in PcapInput\n");
        QDir::setCurrent("..");

		// clear recursively directory
        Toolbox::removeDirectory(tmpPcapDir);
        LOG_ERROR("FAILURE\n\n");
        return false;
	}

	// iterate through all flow files
    ExploitSampleHandle s;
	QDirIterator it(".", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(it.hasNext()) {
		string entryName = it.next().toStdString();
		if(entryName == "report.xml")
            continue;

		// read code
		LOG("opening pcap flow file: [%s]\n", entryName.c_str());
		QFile file(entryName.c_str());
        file.open(QIODevice::ReadOnly);
        if(!file.isOpen())
            continue;

        int size = file.size();

		// protect against bad or too big files
		if(Options::instance()->skipBigFiles) {
			if(size > Options::instance()->bigFileSize) {
				LOG("file [%s] is too big, size: [%d]\n", entryName.c_str(), size);
				LOG("skipping\n");
				file.close();
				file.remove();
				continue;
			}
		}

        char *buffer = new char[size];
		LOG("allocating code buffer for sample, size: [%d]\n", size);

        file.read(buffer, size);
        file.close();

		// create new sample
		s = ExploitSampleHandle(new ExploitSample());
		QFileInfo info(filename.c_str());
        s->info()->setName(Toolbox::pcapFlowBasename(entryName));
		s->info()->setExtractedFrom(info.absoluteFilePath().toStdString());
        s->info()->setFileType(m_type);
		s->info()->setFileSize(size);
        s->setCode((byte_t *) buffer);
        samples->push_back(s);
        file.remove();
    }

	// clean up
    QDir::setCurrent("..");
	LOG("removing directory: [%s]\n", tmpPcapDir.c_str());
    bool removed = Toolbox::removeDirectory(tmpPcapDir);
    if(!removed) {
        SystemLogger::instance()->setError("removing tmp directory failed");
		LOG_ERROR("removing directory failed: [%s]\n", tmpPcapDir.c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }
    LOG("SUCCESS\n\n");
    return true;
}
