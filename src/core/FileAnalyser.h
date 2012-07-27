/*
 * Filename	: FileAnalyser.h
 * Author	: Kuba Sejdak
 * Created on	: 26-07-2012
 */

#ifndef FILEANALYSER_H_
#define FILEANALYSER_H_

/* standard headers */
#include <string>
#include <fstream>
#include <cstring>
#include <stdint.h>
#include <toolbox.h>
using namespace std;

class FileAnalyser {
public:
	FileAnalyser();
	virtual ~FileAnalyser();

	string analyze(string filename);

private:
	bool checkExe();
	bool checkPcap();

	fstream file;
	string type;
};

#endif /* FILEANALYSER_H_ */
