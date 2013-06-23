/*
 *  Filename	: DescriptionExportStrategy.h
 *  Author		: Kuba Sejdak
 *  Created on	: 17-04-2013
 */

#ifndef DESCRIPTIONEXPORTSTRATEGY_H
#define DESCRIPTIONEXPORTSTRATEGY_H

#include <string>

#include <utils/XMLParser.h>

#define STATUS_FILE     "/var/www/jsejdak/GraphAnalyzerStatus.xml"

class IDescriptionExportStrategy {
public:
    IDescriptionExportStrategy() {}
    virtual ~IDescriptionExportStrategy() {}

    virtual void exportDescription() = 0;
	virtual std::string description() = 0;
};

class XMLDescriptionExportStrategy : public IDescriptionExportStrategy {
public:
    XMLDescriptionExportStrategy() {}
    virtual ~XMLDescriptionExportStrategy() {}

    virtual void exportDescription();
	virtual std::string description();

private:
    XMLParser m_xmlParser;
};

#endif /* DESCRIPTIONEXPORTSTRATEGY_H */
