/*
 * Filename		: Toolbox.cpp
 * Author		: Kuba Sejdak
 * Created on	: 20-07-2012
 */

#include "core/Toolbox.h"

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

int ftw_remove_dir(const char *fpath, const struct stat *sb, int typeflag)
{
    return unlink(fpath);
}

void Toolbox::removeDirectory(QString path)
{
    ftw(path.toStdString().c_str(), ftw_remove_dir, 0);
    QDir d;
    d.remove(path);
}
