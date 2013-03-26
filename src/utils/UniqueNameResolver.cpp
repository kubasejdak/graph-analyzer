/*
 *  Filename	: UniqueNameResolver.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 20-03-2013
 */

#include "UniqueNameResolver.h"
#include <utils/SystemLogger.h>

#include <QFile>
#include <QFileInfo>

QString UniqueNameResolver::resolve(QString filename)
{
	QString uniqueName = filename;
	int k = 2;
	while(QFile(uniqueName).exists()) {
		LOG("duplicate for given filename found, renaming\n");
		QFileInfo duplicate(uniqueName);
		uniqueName = QString("%1/%2").arg(duplicate.absolutePath()).arg(duplicate.baseName());

		if(k != 2) {
			int n = uniqueName.lastIndexOf('_');
			uniqueName.truncate(n + 1);
		}
		else
			uniqueName += "_";
		QString str_num;
		uniqueName += str_num.setNum(k) + ".png";
		LOG("renamed to: [%s]\n", uniqueName.toStdString().c_str());
		++k;
	}

	LOG("SUCCESS\n\n");
	return uniqueName;
}
