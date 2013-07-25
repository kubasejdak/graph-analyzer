/*
 * Filename		: IndexFileInput.h
 * Author		: Kuba Sejdak
 * Created on	: 25-08-2013
 */

#ifndef ANALYZE_INDEXFILEINPUT_H
#define ANALYZE_INDEXFILEINPUT_H

#include <string>

#include <tasks/analyze/modules/input/IInput.h>
#include <core/ExploitSample.h>

namespace Analyze {

class IndexFileInput: public IInput {
public:
	IndexFileInput();
	virtual ~IndexFileInput() {}

	virtual bool loadInput(std::string filename, SampleList *samples);
};

} // namespace Analyze

#endif /* ANALYZE_INDEXFILEINPUT_H */
