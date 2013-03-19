/*
 * Filename		: FileTypeAnalyzer.h
 * Author		: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef FILETYPEANALYZER_H_
#define FILETYPEANALYZER_H_

#include <QString>

/// @brief Provides method to determine MIME type of a given file using libmagic library.
class FileTypeAnalyzer {
public:
	/// @brief Default constructor.
	FileTypeAnalyzer() {}

	/// @brief Destructor.
	virtual ~FileTypeAnalyzer() {}

	/// @brief Determines MIME type of a given file.
	/// @param filename Name of the file to be analyzed.
	/// @return MIME type.
    QString analyze(QString filename);
};

#endif /* FILETYPEANALYZER_H_ */
