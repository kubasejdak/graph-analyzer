/*
 *  Filename	: StatusExportStrategy.h
 *  Author		: Kuba Sejdak
 *  Created on	: 02-04-2013
 */

#ifndef STATUSEXPORTSTRATEGY_H
#define STATUSEXPORTSTRATEGY_H

#include <QString>

class IStatusExportStrategy {
public:
    IStatusExportStrategy() {}
    virtual ~IStatusExportStrategy() {}

    virtual void exportStatus(int progress) = 0;
    virtual QString description() = 0;
};

class DBStatusExportStrategy : public IStatusExportStrategy {
public:
    DBStatusExportStrategy() {}
    virtual ~DBStatusExportStrategy() {}

    virtual void exportStatus(int progress);
    virtual QString description();
};

#endif /* STATUSEXPORTSTRATEGY_H */
