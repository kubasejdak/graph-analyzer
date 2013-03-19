/*
 * Filename		: Toolbox.h
 * Author		: Kuba Sejdak
 * Created on	: 09-07-2012
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <QString>
#include <stdint.h>

typedef uint8_t byte_t;

class Toolbox {
public:
    static QString itos(int n, int base = 10);
    static QString hash(QString data);

    /* paths */
    static QString makeRelative(QString path);
    static QString removeSlash(QString path);
    static QString pcapFlowBasename(QString path);
    static bool removeDirectory(QString path);
};

#endif /* TOOLBOX_H_ */
