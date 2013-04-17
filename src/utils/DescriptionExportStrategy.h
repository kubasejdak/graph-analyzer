/*
 *  Filename	: DescriptionExportStrategy.h
 *  Author		: Kuba Sejdak
 *  Created on	: 17-04-2013
 */

#ifndef DESCRIPTIONEXPORTSTRATEGY_H
#define DESCRIPTIONEXPORTSTRATEGY_H

#include <QString>
#include <tasks/ITask.h>
#include <utils/XMLParser.h>

#define STATUS_FILE     "/var/www/jsejdak/GraphAnalyzerStatus.xml"

class IDescriptionExportStrategy {
public:
    IDescriptionExportStrategy() {}
    virtual ~IDescriptionExportStrategy() {}

    virtual void exportDescription() = 0;
    virtual QString description() = 0;
};

class XMLDescriptionExportStrategy : public IDescriptionExportStrategy {
public:
    XMLDescriptionExportStrategy() {}
    virtual ~XMLDescriptionExportStrategy() {}

    virtual void exportDescription();
    virtual QString description();

private:
    XMLParser m_xmlParser;
};

#endif /* DESCRIPTIONEXPORTSTRATEGY_H */
