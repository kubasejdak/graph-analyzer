/*
 *  Filename	: AlgorithmContext.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#include "AlgorithmContext.h"

#include <map>
#include <string>

using namespace std;
using namespace Group;

void AlgorithmContext::setValue(string key, string value)
{
	m_values[key] = value;
}

string AlgorithmContext::value(string key)
{
	return m_values[key];
}

AlgorithmContext::ContextMap &AlgorithmContext::data()
{
	return m_values;
}
