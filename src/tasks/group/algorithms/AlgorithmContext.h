/*
 *  Filename	: AlgorithmContext.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef ALGORITHMCONTEXT_H
#define ALGORITHMCONTEXT_H

#include <map>
#include <string>

class AlgorithmContext {
public:
	AlgorithmContext() {}
	virtual ~AlgorithmContext() {}

	void setValue(std::string key, std::string value);
	std::string value(std::string key);

private:
	std::map<std::string, std::string> m_values;
};

#endif /* ALGORITHMCONTEXT_H */
