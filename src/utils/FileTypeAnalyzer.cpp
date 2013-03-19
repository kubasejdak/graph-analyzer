/*
 * Filename		: FileTypeAnalyzer.cpp
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#include "FileTypeAnalyzer.h"
#include <utils/Toolbox.h>
#include <utils/SystemLogger.h>

#include <magic.h>

QString FileTypeAnalyzer::analyze(QString filename)
{
	magic_t myt = magic_open(MAGIC_ERROR | MAGIC_MIME_TYPE);
	magic_load(myt, NULL);
	QString fileType = magic_file(myt, filename.toStdString().c_str());
	magic_close(myt);

	LOG("recognized file: [%s] as [%s] type\n", filename.toStdString().c_str(), fileType.toStdString().c_str());
	LOG("SUCCESS\n\n");
	return fileType;
}
