/*
 * GeneratorSet.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "GeneratorSet.h"

GeneratorSet::GeneratorSet(const double ratedPower) :
_connected(true), _wasconnected(true), _firstUpdate(true),_powerAvailable(
		0.),  _powerGenerated(0.),_powerGeneratedPrevious(0.), _ratedPower(ratedPower) {
}

GeneratorSet::~GeneratorSet() {
}

void GeneratorSet::updateStatus(bool connected, double powerGenerated,
		double powerAvailable) {
	if (_firstUpdate) {
		_firstUpdate = false;
		_wasconnected = connected;
		_powerGeneratedPrevious = powerGenerated;

	} else {
		_wasconnected = _connected;
		_powerGeneratedPrevious = _powerGenerated;
	}

	_connected = connected;
	_powerGenerated = powerGenerated;
	_powerAvailable = powerAvailable;
}

bool GeneratorSet::isConnected() const{
	return _connected;
}

bool GeneratorSet::wasConnected() const{
	return _wasconnected;
}

bool GeneratorSet::hasDisconnected() const{
	return _connected == false && _wasconnected == true;
}

double GeneratorSet::getPowerAvailable() const{
	return _powerAvailable;
}

double GeneratorSet::getPowerGenerated() const{
	return _powerGenerated;
}

double GeneratorSet::getPowerGeneratedPrevious() const{
	return _powerGeneratedPrevious;
}

double GeneratorSet::getPowerRated() const{
	return _ratedPower;
}
