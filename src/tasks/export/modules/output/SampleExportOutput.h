/*
 * Filename		: SampleExportOutput.h
 * Author		: Kuba Sejdak
 * Created on	: 19-07-2013
 */

#ifndef EXPORT_SAMPLEEXPORTOUTPUT_H
#define EXPORT_SAMPLEEXPORTOUTPUT_H

#include <tasks/export/modules/output/IOutput.h>

class ExploitSample;
class ExploitInfo;

namespace Export {

class SampleExportOutput: public IOutput {
public:
	SampleExportOutput();
	virtual ~SampleExportOutput() {}

	virtual bool exportOutput(ExploitSampleHandle sample, std::string exportDir, bool onlyIndexFile);
};

} // namespace Export

#endif /* EXPORT_SAMPLEEXPORTOUTPUT_H */
