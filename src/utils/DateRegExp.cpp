/*
 *  Filename	: DateRegExp.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "DateRegExp.h"

#include <string>
#include <algorithm>
#include <QDate>
#include <QRegExp>

using namespace std;

QDate DateRegExp::findDate(string name)
{
	QRegExp rx("/[0-9]{8}/");
	rx.indexIn(name.c_str());
	string dateStr = rx.cap().toStdString();
	dateStr.erase (remove(dateStr.begin(), dateStr.end(), '/'), dateStr.end());
	QDate date =  QDate::fromString(dateStr.c_str(), "yyyyMMdd");

	if(date.isValid())
		return date;
	else
		return QDate(1999, 12, 31);
}
