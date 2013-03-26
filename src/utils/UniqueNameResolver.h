/*
 *  Filename	: UniqueNameResolver.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef UNIQUENAMERESOLVER_H
#define UNIQUENAMERESOLVER_H

#include <QString>

class UniqueNameResolver {
public:
	UniqueNameResolver() {}
	virtual ~UniqueNameResolver() {}

	QString resolve(QString filename);
};

#endif /* UNIQUENAMERESOLVER_H */
