/*
 * Filename		: InstructionSplitter.h
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#ifndef INSTRUCTIONSPLITTER_H_
#define INSTRUCTIONSPLITTER_H_

#include <QString>
#include <vector>
#include <string>
using namespace std;

class InstructionSplitter {
public:
	InstructionSplitter() {}
	virtual ~InstructionSplitter() {}

    QString address();
    QString code();
    QString syscall();
    QString instr();
    QString firstArg();
    QString secondArg();
	void clear();

    InstructionSplitter &operator=(QString s);

private:
    vector<string> m_holder;
};

#endif /* INSTRUCTIONSPLITTER_H_ */