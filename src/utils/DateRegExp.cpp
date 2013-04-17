/*
 *  Filename	: DateRegExp.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "DateRegExp.h"

#include <QRegExp>

QDate DateRegExp::findDate(QString name)
{
	QRegExp rx("/[0-9]{8}/");
	rx.indexIn(name);
	QString dateStr = rx.cap();
	dateStr.remove('/');
	QDate date =  QDate::fromString(dateStr, "yyyyMMdd");

	if(date.isValid())
		return date;
	else
		return QDate(1999, 12, 31);
}
