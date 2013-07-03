/*
 *  Filename	: BinaryInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 08-05-2012
 */

#include "BinaryInput.h"

#include <string>
#include <QFileInfo>

#include <tasks/analyze/modules/input/IInput.h>
#include <core/ExploitSample.h>
#include <core/Options.h>
#include <utils/SystemLogger.h>

using namespace std;

BinaryInput::BinaryInput()
{
    m_name = "BinaryInput";
	m_type = "application/octet-stream";
	m_description = "Loads exploit from binary files.";
}

bool BinaryInput::loadInput(string filename, SampleList *samples)
{
	QFile file(filename.c_str());
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) {
        LOG_ERROR("FAILURE\n\n");
        return false;
    }

	unsigned long size = file.size();

	// protect against bad or too big files
	if(Options::instance()->skipBigFiles) {
		if(size > Options::instance()->bigFileSize) {
			LOG("file [%s] is too big, size: [%lu]\n", filename.c_str(), size);
			LOG("skipping\n");
			LOG("SUCCESS\n\n");
			return true;
		}
	}

	char *buffer = new char[size];
	LOG("allocating code buffer for sample, size: [%lu]\n", size);

	file.read(buffer, size);
	file.close();

	ExploitSampleHandle s = ExploitSampleHandle(new ExploitSample());
	QFileInfo info(filename.c_str());
	s->info()->setName(info.absoluteFilePath().toStdString());
	s->info()->setExtractedFrom(info.absoluteFilePath().toStdString());
    s->info()->setFileType(m_type);
	s->info()->setFileSize(size);
	s->setCode((byte_t *) buffer);
	samples->push_back(s);
    LOG("SUCCESS\n\n");
    return true;
}
