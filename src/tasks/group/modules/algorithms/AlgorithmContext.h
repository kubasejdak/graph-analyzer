/*
 *  Filename	: AlgorithmContext.h
 *  Author		: Kuba Sejdak
 *  Created on	: 07-07-2013
 */

#ifndef GROUP_ALGORITHMCONTEXT_H
#define GROUP_ALGORITHMCONTEXT_H

#include <map>
#include <string>

namespace Group {

class AlgorithmContext {
public:
	AlgorithmContext() {}
	virtual ~AlgorithmContext() {}

	void setValue(std::string key, std::string value);
	std::string value(std::string key);

private:
	std::map<std::string, std::string> m_values;
};

} // namespace Group

#endif /* GROUP_ALGORITHMCONTEXT_H */
