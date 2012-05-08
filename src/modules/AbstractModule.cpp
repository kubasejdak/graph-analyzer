/*
 *  Filename	: AbstractModule.cpp
 *  Author		: Kuba Sejdak
 *  Created on	: 01-05-2012
 */

#include "AbstractModule.h"

AbstractModule::AbstractModule() {
}

AbstractModule::~AbstractModule() {
}

int AbstractModule::getId() {
	return id;
}

string AbstractModule::getName() {
	return name;
}

string AbstractModule::getDescription() {
	return description;
}
