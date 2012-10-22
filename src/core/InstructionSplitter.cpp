/*
 * Filename		: InstructionSplitter.cpp
 * Author		: Kuba Sejdak
 * Created on	: 12-07-2012
 */

#include "InstructionSplitter.h"

InstructionSplitter::InstructionSplitter()
{
}

InstructionSplitter::~InstructionSplitter()
{
}

QString InstructionSplitter::address()
{
    if(m_holder.size() < 1)
		return "";

    return m_holder[0];
}

QString InstructionSplitter::code()
{
    if(m_holder.size() < 2)
		return "";

    return m_holder[1];
}

QString InstructionSplitter::syscall()
{
    if(m_holder.size() != 2)
		return "";

    return m_holder[1];
}

QString InstructionSplitter::instr()
{
    if(m_holder.size() < 3)
		return "";

    return m_holder[2];
}

QString InstructionSplitter::firstArg()
{
    if(m_holder.size() < 4)
		return "";

    return m_holder[3];
}

QString InstructionSplitter::secondArg()
{
    if(m_holder.size() < 5)
		return "";

    return m_holder[4];
}

InstructionSplitter &InstructionSplitter::operator=(QString s)
{
    m_holder.clear();
    s.chop(2);

    m_holder = s.split(" ,");
    //split(m_holder, s, is_any_of(" ,"), token_compress_on);

	return *this;
}

void InstructionSplitter::clear()
{
    m_holder.clear();
}
