/*
 * Thruster.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "Thruster.h"

Thruster::Thruster(const double &ratedPower) {
	_ratedPower = ratedPower;
}

Thruster::~Thruster() {
}

double Thruster::getPower() const{
	return _load;
}


double Thruster::getPowerAvailable() const{
	return _powerAvailable;
}

void Thruster::setPower(double load) {
	_load = load;
}

double Thruster::getPowerRated() const{
	return _ratedPower;
}

void Thruster::setFLRPowerAvailable(double powerAvaiable) {
	_powerAvailable = powerAvaiable;
}
