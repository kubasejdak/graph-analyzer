/*
 *  Filename	: AbstractInput.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#include "AbstractInput.h"

int AbstractInput::nextID = INPUT_ID_OFFSET;

AbstractInput::AbstractInput()
{
}

AbstractInput::~AbstractInput()
{
}

string AbstractInput::getType()
{
	return type;
}
