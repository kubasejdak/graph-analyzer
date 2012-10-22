/*
 * Filename		: ConsoleOutput.cpp
 * Author		: Kuba Sejdak
 * Created on	: 23-08-2012
 */

#include "ConsoleOutput.h"

ConsoleOutput::ConsoleOutput()
{
    m_name = "ConsoleOutput";
    m_description = "Prints info about samples in console.";
}

bool ConsoleOutput::generateOutput(ShellcodeSample *sample)
{
	cout << "Results for sample:" << endl;
    sample->info()->printInfo();
	cout << endl;

	return true;
}
