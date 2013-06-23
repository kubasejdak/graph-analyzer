/*
 *  Filename	: UniqueNameResolver.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef UNIQUENAMERESOLVER_H
#define UNIQUENAMERESOLVER_H

#include <string>

class UniqueNameResolver {
public:
	UniqueNameResolver() {}
	virtual ~UniqueNameResolver() {}

	std::string resolve(std::string filename);
};

#endif /* UNIQUENAMERESOLVER_H */
