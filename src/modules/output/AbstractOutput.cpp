/*
 *  Filename	: AbstractOutput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#include "AbstractOutput.h"

int AbstractOutput::m_nextID = OUTPUT_ID_OFFSET;

AbstractOutput::AbstractOutput()
{
    m_id = nextID();
}
