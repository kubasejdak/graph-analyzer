/*
 * Filename		: GraphAnalyzerPython.cpp
 * Author		: Kuba Sejdak
 * Created on	: 06-09-2012
 */

#include <boost/python.hpp>
#include <core/CoreSystem.h>
#include <python2.7/Python.h>
using namespace boost::python;

BOOST_PYTHON_MODULE(GraphAnalyzerPython)
{
	class_<CoreSystem>("CoreSystem")
		/* basic sample operation */
		.def("addFile", &CoreSystem::addFile)
		.def("run", &CoreSystem::run)
		.def("clear", &CoreSystem::clear)

		/* status and error */
		.def("getStatus", &CoreSystem::getStatus)
		.def("getError", &CoreSystem::getError)

		/* logging */
		.def("setLogFile", &CoreSystem::setLogFile)
		.def("setLogLevel", &CoreSystem::setLogLevel)

		/* results */
		.def("setOutput", &CoreSystem::setOutput)
		.def("getExploitsNum", &CoreSystem::getExploitsNum)
		.def("getSamplesNum", &CoreSystem::getSamplesNum)

		/* utility */
		.def("getVersion", &CoreSystem::getVersion)
	;
}
