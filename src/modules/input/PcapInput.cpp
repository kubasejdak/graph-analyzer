/*
 * Filename		: PcapInput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "PcapInput.h"

PcapInput::PcapInput()
{
    m_name = "PcapInput";
    m_type = "pcap";
	m_description = "Loads exploit from pcap files.";
}

bool PcapInput::loadInput(QString filename, QList<ExploitSample *> *samples)
{
    int success;

    /* move to /tmp/pcap_tmp */
    QDir pcapDir;
    if(!QDir("/tmp/pcap_tmp").exists()) {
        success = pcapDir.mkdir("/tmp/pcap_tmp");
        if(!success) {
            LOG_ERROR("cannot create /tmp/pcap_tmp directory\n");
            LOG_ERROR("FAILURE\n\n");
            return false;
		}
	}

    success = QDir::setCurrent("/tmp/pcap_tmp");
    if(!success) {
        LOG_ERROR("cannot change directory to /tmp/pcap_dir");
        Toolbox::removeDirectory("tmp/pcap_tmp");
        return false;
	}

	/* create flow files */
    QFileInfo f(filename);
    QString tcpflowCmd = QString("tcpflow -r \"%1\"").arg(f.absoluteFilePath());

    int ret = system(tcpflowCmd.toStdString().c_str());
    if(ret) {
        SystemLogger::instance()->setError("external program 'tcpflow' failed");
		LOG_ERROR("tcpflow cmd failed in PcapInput");
        QDir::setCurrent("..");

        /* clear recursively directory */
        Toolbox::removeDirectory("/tmp/pcap_tmp");
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
		if(Options::instance()->PROTECT_AGAINST_BIG_FILES) {
			if(size > Options::instance()->MAX_INPUT_FILE_SIZE) {
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
    LOG("removing directory: [/tmp/pcap_tmp]\n");
    bool removed = Toolbox::removeDirectory("/tmp/pcap_tmp");
    if(!removed) {
        SystemLogger::instance()->setError("removing directory '/tmp/pcap_tmp']' failed");
        LOG_ERROR("removing directory failed: [/tmp/pcap_tmp]\n");
        LOG_ERROR("FAILURE\n\n");
        return false;
    }
    LOG("SUCCESS\n\n");
    return true;
}
