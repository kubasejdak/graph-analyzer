/*
 *  Filename	: StatusExportStrategy.h
 *  Author		: Kuba Sejdak
 *  Created on	: 02-04-2013
 */

#ifndef STATUSEXPORTSTRATEGY_H
#define STATUSEXPORTSTRATEGY_H

#include <string>

class ITask;

#define STATUS_FILE     "/var/www/jsejdak/GraphAnalyzerStatus.xml"

class IStatusExportStrategy {
public:
    IStatusExportStrategy() {}
    virtual ~IStatusExportStrategy() {}

    virtual void exportStatus(ITask *task) = 0;
	virtual std::string description() = 0;
};

class DBStatusExportStrategy : public IStatusExportStrategy {
public:
    DBStatusExportStrategy() {}
    virtual ~DBStatusExportStrategy() {}

    virtual void exportStatus(ITask *task);
	virtual std::string description();
};

#endif /* STATUSEXPORTSTRATEGY_H */
