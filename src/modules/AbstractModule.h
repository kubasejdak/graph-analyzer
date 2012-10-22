/*
 *  Filename	: AbstractModule.h
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#ifndef ABSTRACTMODULE_H_
#define ABSTRACTMODULE_H_

#define INPUT_ID_OFFSET		100
#define OUTPUT_ID_OFFSET	200
#define ANALYZE_ID_OFFSET	300

#include <QString>

#include <modules/ModuleInfo.h>

class AbstractModule {
public:
    AbstractModule() {}
    virtual ~AbstractModule() {}

    int id();
    QString name();
    QString description();
    ModuleInfo *moduleInfo();

protected:
    int m_id;
    QString m_name;
    QString m_description;
    ModuleInfo m_info;
};

#endif /* ABSTRACTMODULE_H_ */
