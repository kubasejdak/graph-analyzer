/*
 * Filename		: InstructionSplitter.cpp
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#include "InstructionSplitter.h"

QString InstructionSplitter::address()
{
    if(m_holder.size() < 1)
		return "";

    return m_holder[0].c_str();
}

QString InstructionSplitter::code()
{
    if(m_holder.size() < 2)
		return "";

    return m_holder[1].c_str();
}

QString InstructionSplitter::syscall()
{
    if(m_holder.size() != 2)
		return "";

    return m_holder[1].c_str();
}

QString InstructionSplitter::instr()
{
    if(m_holder.size() < 3)
		return "";

    return m_holder[2].c_str();
}

QString InstructionSplitter::firstArg()
{
    if(m_holder.size() < 4)
		return "";

    return m_holder[3].c_str();
}

QString InstructionSplitter::secondArg()
{
    if(m_holder.size() < 5)
		return "";

    return m_holder[4].c_str();
}

InstructionSplitter &InstructionSplitter::operator=(QString s)
{
    m_holder.clear();
    s.chop(2);
    string stdStr = s.toStdString();
    split(m_holder, stdStr, is_any_of(" ,"), token_compress_on);

	return *this;
}

void InstructionSplitter::clear()
{
    m_holder.clear();
}
