/*
 *  Filename	: AnalyzeTask.h
 *  Author		: Kuba Sejdak
 *  Created on	: 14-03-2013
 */

#ifndef ANALYZETASK_H
#define ANALYZETASK_H

#include <tasks/ITask.h>

class AnalyzeTask : public ITask {
public:
	AnalyzeTask() {}
	virtual ~AnalyzeTask() {}

	virtual bool perform();

private:
	bool readConfigXML();
};

#endif /* ANALYZETASK_H */
