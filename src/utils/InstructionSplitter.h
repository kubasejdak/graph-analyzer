/*
 * Filename		: InstructionSplitter.h
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#ifndef INSTRUCTIONSPLITTER_H_
#define INSTRUCTIONSPLITTER_H_

#include <vector>
#include <string>

class InstructionSplitter {
public:
	InstructionSplitter() {}
	virtual ~InstructionSplitter() {}

	std::string address();
	std::string code();
	std::string syscall();
	std::string instr();
	std::string firstArg();
	std::string secondArg();
	void clear();

	InstructionSplitter &operator=(std::string s);

private:
	std::vector<std::string> m_holder;
};

#endif /* INSTRUCTIONSPLITTER_H_ */
