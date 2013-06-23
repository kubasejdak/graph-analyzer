/*
 *  Filename	: IInput.h
 *  Author		: Kuba Sejdak
 *  Created on	: 06-05-2012
 */

#ifndef IINPUT_H_
#define IINPUT_H_

#include <string>

#include <tasks/analyze/modules/IModule.h>
#include <core/ExploitSample.h>

class IInput: public IModule {
public:
	IInput() {}
	virtual ~IInput() {}

	std::string type() { return m_type; }
	virtual bool loadInput(std::string filename, SampleContainer *samples) = 0;

protected:
	std::string m_type;
};

#endif /* IINPUT_H_ */
