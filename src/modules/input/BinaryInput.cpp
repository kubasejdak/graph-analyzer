/*
 *  Filename	: BinaryInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "BinaryInput.h"

BinaryInput::BinaryInput()
{
    m_name = "BinaryInput";
    m_type = "binary";
    m_description = "Loads shellcode from binary files.";
}

bool BinaryInput::loadInput(QString filename, QList<ShellcodeSample *> *samples)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) {
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

    int size = file.size();
	char *buffer = new char[size];
	file.read(buffer, size);
	file.close();

	ShellcodeSample *s = new ShellcodeSample();
    QFileInfo info(filename);
    s->info()->setName(info.absoluteFilePath());
    s->info()->setExtractedFrom(info.absoluteFilePath());
    s->info()->setFileType(m_type);
    s->info()->setSize(size);
	s->setCode((byte_t *) buffer);
	samples->push_back(s);
    LOG("SUCCESS\n\n");
    return true;
}
