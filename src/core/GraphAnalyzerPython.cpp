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
		.def("getVersion", &CoreSystem::getVersion)
	;
}
