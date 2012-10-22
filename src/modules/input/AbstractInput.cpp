/*
 *  Filename	: AbstractInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#include "AbstractInput.h"

int AbstractInput::m_nextID = INPUT_ID_OFFSET;

AbstractInput::AbstractInput()
{
    m_id = nextID();
}

QString AbstractInput::type()
{
    return m_type;
}
