/*
 * Filename		: ConsoleOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#include "ConsoleOutput.h"

ConsoleOutput::ConsoleOutput()
{
	id = getNextID();
	name = "ConsoleOutput";
	description = "Prints info about samples in console.";
}

ConsoleOutput::~ConsoleOutput()
{
}

bool ConsoleOutput::generateOutput(ShellcodeSample *sample)
{
	cout << "Results for sample:" << endl;
	sample->getInfo()->printInfo();
	cout << endl;

	return true;
}
