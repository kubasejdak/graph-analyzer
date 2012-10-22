/*
 *  Filename	: AbstractModule.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "AbstractModule.h"

int AbstractModule::id()
{
    return m_id;
}

QString AbstractModule::name()
{
    return m_name;
}

QString AbstractModule::description()
{
    return m_description;
}

ModuleInfo *AbstractModule::moduleInfo()
{
    m_info.m_id = m_id;
    m_info.m_name = m_name;
    m_info.m_description = m_description;

    return &m_info;
}
