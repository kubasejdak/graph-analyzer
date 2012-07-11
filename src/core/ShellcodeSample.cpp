/*
 *  Filename	: ShellcodeSample.cpp
 *  Author	: Kuba Sejdak
 *  Created on	: 03-05-2012
 */

#include "ShellcodeSample.h"

ShellcodeSample::ShellcodeSample() {
	code = NULL;
	graph = new Graph();
	info = new ShellcodeInfo();
}

ShellcodeSample::~ShellcodeSample() {
	delete graph;
	delete info;
	if(code)
		delete code;
}

byte_t *ShellcodeSample::getCode() {
	return code;
}

ShellcodeInfo *ShellcodeSample::getInfo() {
	return info;
}

Graph *ShellcodeSample::getGraph() {
	return graph;
}

void ShellcodeSample::setCode(byte_t *code) {
	this->code = code;
}
