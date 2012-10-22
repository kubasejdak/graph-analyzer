/*
 *  Filename	: AbstractAnalyze.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-05-2012
 */

#include "AbstractAnalyze.h"

int AbstractAnalyze::m_nextID = ANALYZE_ID_OFFSET;

AbstractAnalyze::AbstractAnalyze()
{
    m_id = nextID();
}
