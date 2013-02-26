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

class AbstractModule {
public:
    AbstractModule() {}
    virtual ~AbstractModule() {}

	int id() { return m_id; }
	QString name() { return m_name; }
	QString description() { return m_description; }

protected:
    int m_id;
    QString m_name;
    QString m_description;
};

#endif /* ABSTRACTMODULE_H_ */
