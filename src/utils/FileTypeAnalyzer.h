/*
 * Filename		: FileTypeAnalyzer.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef FILETYPEANALYZER_H_
#define FILETYPEANALYZER_H_

#include <QString>

class FileTypeAnalyzer {
public:
	FileTypeAnalyzer() {}
	virtual ~FileTypeAnalyzer() {}

    QString analyze(QString filename);
};

#endif /* FILETYPEANALYZER_H_ */
