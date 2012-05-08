/*
 *  Filename	: AnalysisSystem.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "AnalysisSystem.h"

AnalysisSystem::AnalysisSystem() {
	inputModules = ModuleManager::getInstance()->getInput();
	outputModules = ModuleManager::getInstance()->getOutput();
}

AnalysisSystem::~AnalysisSystem() {
}

bool AnalysisSystem::loadShellcode(string filename) {
	return true;
}

bool AnalysisSystem::analyze(string filename) {
	return true;
}

ShellcodeInfo AnalysisSystem::getResults(string filename) {
	return ShellcodeInfo();
}

bool AnalysisSystem::generateOutput(string filename, int method, string *output) {
	return true;
}

SystemStatus AnalysisSystem::getStatus() {
	return status;
}

void AnalysisSystem::clearCache() {
}

void AnalysisSystem::loadModules() {
}

void AnalysisSystem::changeStatus(SystemStatus status) {
	this->status = status;
}
