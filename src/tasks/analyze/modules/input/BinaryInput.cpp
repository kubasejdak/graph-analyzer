/*
 *  Filename	: BinaryInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "BinaryInput.h"
#include <core/Options.h>

#include <QFileInfo>

BinaryInput::BinaryInput()
{
    m_name = "BinaryInput";
	m_type = "application/octet-stream";
	m_description = "Loads exploit from binary files.";
}

bool BinaryInput::loadInput(QString filename, QList<ExploitSample *> *samples)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) {
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

	int size = file.size();

	/* protect against bad or too big files */
	if(Options::instance()->skipBigFiles) {
		if(size > Options::instance()->bigFileSize) {
			LOG("file [%s] is too big, size: [%d]\n", filename.toStdString().c_str(), size);
			LOG("skipping\n");
			LOG("SUCCESS\n\n");
			return true;
		}
	}

	char *buffer = new char[size];
	LOG("allocating code buffer for sample, size: [%d]\n", size);

	file.read(buffer, size);
	file.close();

	ExploitSample *s = new ExploitSample();
    QFileInfo info(filename);
    s->info()->setName(info.absoluteFilePath());
    s->info()->setExtractedFrom(info.absoluteFilePath());
    s->info()->setFileType(m_type);
	s->info()->setFileSize(size);
	s->setCode((byte_t *) buffer);
	samples->push_back(s);
    LOG("SUCCESS\n\n");
    return true;
}
