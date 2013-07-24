/*
 *  Filename	: IOutput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef EXPORT_IOUTPUT_H_
#define EXPORT_IOUTPUT_H_

#include <string>

#include <tasks/export/modules/IModule.h>
#include <core/ExploitSample.h>

namespace Export {

class IOutput: public IModule {
public:
	IOutput() {}
	virtual ~IOutput() {}

	virtual bool exportOutput(ExploitSampleHandle sample, std::string exportDir, bool onlyIndexFile) = 0;
};

} // namespace Export

#endif /* EXPORT_IOUTPUT_H_ */
