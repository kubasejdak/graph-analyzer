/*
 * Filename	: InstructionSplitter.h
 * Author	: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#ifndef INSTRUCTIONSPLITTER_H_
#define INSTRUCTIONSPLITTER_H_

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;

class InstructionSplitter {
public:
	InstructionSplitter();
	virtual ~InstructionSplitter();

	string getAddress();
	string getCode();
	string getSyscall();
	string getInstr();
	string getFirstArg();
	string getSecondArg();
	void clear();

	InstructionSplitter &operator=(string s);

private:
	vector<string> holder;
};

#endif /* INSTRUCTIONSPLITTER_H_ */
