/*
 * Filename		: Toolbox.cpp
 * Author		: Kuba Sejdak
 * Created on	: 20-07-2012
 */

#include "Toolbox.h"

#include <QFileInfo>
#include <QDir>
#include <stdint.h>
#include <string>
using namespace std;

QString Toolbox::itos(int n, int base)
{
    QString s;
    return QString("%1%2").arg((base == 16) ? "0x" : "").arg(s.setNum(n, base));
}

QString Toolbox::hash(QString data)
{
	/* Fowler–Noll–Vo (FNV-1) hash function */
    string str = data.toStdString();
	int h = 2166136261U;
    for(unsigned i = 0; i < str.size(); ++i)
        h = (16777619U * h) ^ (uint8_t) str[i];

	if(h < 0)
		h = -h;
    return itos(h, 16);
}

QString Toolbox::makeRelative(QString path)
{
    QFileInfo f(path);
    return f.absoluteFilePath();
}

QString Toolbox::removeSlash(QString path)
{
    if(path.endsWith('/'))
        path.chop(1);

	return path;
}

QString Toolbox::pcapFlowBasename(QString path)
{
    int n = path.lastIndexOf("/");
    return path.mid(n + 1);
}

bool Toolbox::removeDirectory(QString path)
{
    return QDir().rmdir(path);
}
