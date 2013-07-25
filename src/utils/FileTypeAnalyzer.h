/*
 * Filename		: FileTypeAnalyzer.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef FILETYPEANALYZER_H_
#define FILETYPEANALYZER_H_

#include <string>

class FileTypeAnalyzer {
public:
	FileTypeAnalyzer() {}
	virtual ~FileTypeAnalyzer() {}

	std::string analyze(std::string filename);

private:
	std::string recognizeInternalType(std::string filename);
	std::string recognizeMIMEType(std::string filename);
};

#endif /* FILETYPEANALYZER_H_ */
