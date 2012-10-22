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
    m_description = "Loads shellcode from pcap files.";
}

void PcapInput::loadInput(QString filename, QList<ShellcodeSample *> *samples)
{
    int success;

    /* move to /tmp/pcap_tmp */
    QDir pcapDir;
    if(!QDir("/tmp/pcap_tmp").exists()) {
        success = pcapDir.mkdir("/tmp/pcap_tmp");
        if(!success) {
            LOG_ERROR("cannot create /tmp/pcap_tmp directory\n");
            exit(1);
		}
	}

    success = QDir::setCurrent("/tmp/pcap_tmp");
    if(!success) {
        LOG_ERROR("cannot change directory to /tmp/pcap_dir");
        Toolbox::removeDirectory("tmp/pcap_tmp");
        exit(1);
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
        exit(1);
	}

    /* iterate through all flow files */
    ShellcodeSample *s;
    QDirIterator it(".", QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QString entryName = it.next();
        if(entryName == "." || entryName == ".." || entryName == "report.xml")
            continue;

        /* read code */
        QFile file(entryName);
        file.open(QIODevice::ReadOnly);
        if(!file.isOpen())
            continue;

        int size = file.size();
        char *buffer = new char[size];

        file.read(buffer, size);
        file.close();

        /* create new sample */
        s = new ShellcodeSample();
        s->info()->setName(entryName);
        s->info()->setExtractedFrom(filename);
        s->info()->setFileType(m_type);
        s->info()->setSize(size);
        s->setCode((byte_t *) buffer);
        samples->push_back(s);
    }

    /* clean up */
    QDir::setCurrent("..");
    Toolbox::removeDirectory("/tmp/pcap_tmp");
}
