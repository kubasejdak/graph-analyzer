/*
 * Filename		: InstructionSplitter.cpp
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#include "InstructionSplitter.h"

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;

string InstructionSplitter::address()
{
    if(m_holder.size() < 1)
		return "";

	return m_holder[0];
}

string InstructionSplitter::code()
{
    if(m_holder.size() < 2)
		return "";

	return m_holder[1];
}

string InstructionSplitter::syscall()
{
    if(m_holder.size() != 2)
		return "";

	return m_holder[1];
}

string InstructionSplitter::instr()
{
    if(m_holder.size() < 3)
		return "";

	return m_holder[2];
}

string InstructionSplitter::firstArg()
{
    if(m_holder.size() < 4)
		return "";

	return m_holder[3];
}

string InstructionSplitter::secondArg()
{
    if(m_holder.size() < 5)
		return "";

	return m_holder[4];
}

InstructionSplitter &InstructionSplitter::operator=(string s)
{
    m_holder.clear();
	s = s.substr(0, s.size() - 2);
	boost::split(m_holder, s, boost::is_any_of(" ,"), boost::token_compress_on);

	return *this;
}

void InstructionSplitter::clear()
{
    m_holder.clear();
}
