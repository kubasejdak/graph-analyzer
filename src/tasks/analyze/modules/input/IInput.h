/*
 *  Filename	: IInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef ANALYZE_IINPUT_H_
#define ANALYZE_IINPUT_H_

#include <string>

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>

namespace Analyze {

class IInput: public IModule {
public:
	IInput() {}
	virtual ~IInput() {}

	std::string type() { return m_type; }
	virtual bool loadInput(std::string filename, SampleList *samples) = 0;

protected:
	std::string m_type;
};

} // namespace Analyze

#endif /* ANALYZE_IINPUT_H_ */
