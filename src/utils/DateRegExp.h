/*
 *  Filename	: DateRegExp.h
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#ifndef DATEREGEXP_H
#define DATEREGEXP_H

#include <string>
#include <QDate>

class DateRegExp {
public:
	DateRegExp() {}
	virtual ~DateRegExp() {}

	QDate findDate(std::string name);
};

#endif /* DATEREGEXP_H */
