/*
 * Filename		: PcapInput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "PcapInput.h"
#include <core/Options.h>
#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>

#include <QDir>
#include <QFileInfo>

PcapInput::PcapInput()
{
    m_name = "PcapInput";
	m_type = "application/vnd.tcpdump.pcap";
	m_description = "Loads exploit from pcap files.";
}

bool PcapInput::loadInput(QString filename, QList<ExploitSample *> *samples)
{
    int success;
    QString tmpPcapDir = Options::instance()->tmpPcapDir;

    /* move to /tmp/pcap_tmp */
    QDir pcapDir;
    if(!QDir(tmpPcapDir).exists()) {
        success = pcapDir.mkdir(tmpPcapDir);
        if(!success) {
            LOG_ERROR("cannot create [%s] directory\n", tmpPcapDir.toStdString().c_str());
            LOG_ERROR("FAILURE\n\n");
            return false;
		}
	}

    success = QDir::setCurrent(tmpPcapDir);
    if(!success) {
        LOG_ERROR("cannot change directory to [%s]\n", tmpPcapDir.toStdString().c_str());
        Toolbox::removeDirectory(tmpPcapDir);
		LOG_ERROR("FAILURE\n\n");
        return false;
	}

	/* create flow files */
    QFileInfo f(filename);
    QString tcpflowCmd = QString("tcpflow -r \"%1\" %2").arg(f.absoluteFilePath()).arg(Options::instance()->tcpflowParams);

    int ret = system(tcpflowCmd.toStdString().c_str());
    if(ret != 0) {
        SystemLogger::instance()->setError("external program 'tcpflow' failed");
        LOG_ERROR("tcpflow cmd failed in PcapInput\n");
        QDir::setCurrent("..");

        /* clear recursively directory */
        Toolbox::removeDirectory(tmpPcapDir);
        LOG_ERROR("FAILURE\n\n");
        return false;
	}

    /* iterate through all flow files */
    ExploitSample *s;
	QDirIterator it(".", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QString entryName = it.next();
		if(entryName == "report.xml")
            continue;

        /* read code */
        LOG("opening pcap flow file: [%s]\n", entryName.toStdString().c_str());
        QFile file(entryName);
        file.open(QIODevice::ReadOnly);
        if(!file.isOpen())
            continue;

        int size = file.size();

		/* protect against bad or too big files */
		if(Options::instance()->skipBigFiles) {
			if(size > Options::instance()->bigFileSize) {
				LOG("file [%s] is too big, size: [%d]\n", entryName.toStdString().c_str(), size);
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

        /* create new sample */
        s = new ExploitSample();
        QFileInfo info(filename);
        s->info()->setName(Toolbox::pcapFlowBasename(entryName));
        s->info()->setExtractedFrom(info.absoluteFilePath());
        s->info()->setFileType(m_type);
		s->info()->setFileSize(size);
        s->setCode((byte_t *) buffer);
        samples->push_back(s);
        file.remove();
    }

    /* clean up */
    QDir::setCurrent("..");
    LOG("removing directory: [%s]\n", tmpPcapDir.toStdString().c_str());
    bool removed = Toolbox::removeDirectory(tmpPcapDir);
    if(!removed) {
        SystemLogger::instance()->setError("removing tmp directory failed");
        LOG_ERROR("removing directory failed: [%s]\n", tmpPcapDir.toStdString().c_str());
        LOG_ERROR("FAILURE\n\n");
        return false;
    }
    LOG("SUCCESS\n\n");
    return true;
}
