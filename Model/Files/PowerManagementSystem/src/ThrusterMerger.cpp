/*
 * ThrusterMerger.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "ThrusterMerger.h"

ThrusterMerger::ThrusterMerger() {
	
}

ThrusterMerger::~ThrusterMerger() {
}

void ThrusterMerger::clear() {
	_thruster.clear();
}

void ThrusterMerger::addThruster(Thruster* thruster) {
	_thruster.push_back(thruster);
}

double ThrusterMerger::getPowerAvailable() const{
	double r = 0;
	for(unsigned int i = 0; i<_thruster.size(); i++)
		r += _thruster[i]->getPowerAvailable();
	return r;
}

double ThrusterMerger::getPower() const{
	double r = 0;
	for(unsigned int i = 0; i<_thruster.size(); i++)
		r += _thruster[i]->getPower();
	return r;
}

Thruster* ThrusterMerger::getThruster(const unsigned long i) {
	return _thruster[i];
}

const Thruster* ThrusterMerger::getThruster(const unsigned long i) const{
    return _thruster[i];
}

unsigned long ThrusterMerger::size() const{
	return _thruster.size();
}
