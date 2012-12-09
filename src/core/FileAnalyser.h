/*
 * Filename		: FileAnalyser.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef FILEANALYSER_H_
#define FILEANALYSER_H_

#include <QString>
#include <magic.h>

#include <core/Toolbox.h>

class FileAnalyser {
public:
	FileAnalyser() {}
	virtual ~FileAnalyser() {}

    QString analyze(QString filename);
};

#endif /* FILEANALYSER_H_ */
