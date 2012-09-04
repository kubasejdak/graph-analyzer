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

/* standard headers */
#include <string>
using namespace std;

/* project headers */
#include <modules/ModuleInfo.h>

class AbstractModule {
public:
	AbstractModule();
	virtual ~AbstractModule();

	int getId();
	string getName();
	string getDescription();
	ModuleInfo *getModuleInfo();

protected:
	int id;
	string name;
	string description;
	ModuleInfo info;
};

#endif /* ABSTRACTMODULE_H_ */
