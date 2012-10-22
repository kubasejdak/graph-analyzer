/*
 *  Filename	: ShellcodeSample.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#include "ShellcodeSample.h"

ShellcodeSample::ShellcodeSample()
{
    m_code = NULL;
    m_graph = new Graph();
    m_info = new ShellcodeInfo();
}

ShellcodeSample::~ShellcodeSample()
{
    delete m_graph;
    delete m_info;
    if(m_code)
        delete m_code;
}

byte_t *ShellcodeSample::code()
{
    return m_code;
}

ShellcodeInfo *ShellcodeSample::info()
{
    return m_info;
}

Graph *ShellcodeSample::graph()
{
    return m_graph;
}

void ShellcodeSample::setCode(byte_t *code)
{
    m_code = code;
}
