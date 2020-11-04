/*
 * DPLoad.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "DPLoad.h"
#include <algorithm>
#ifdef IO_ON
#include <iostream>
#endif

DPLoad::DPLoad(const vector<Thruster> *thrusters, double desiredLoadBias,
		double desiredLoadGain) :
		_thrusters(thrusters), _desiredLoadGain(desiredLoadGain) {
	if (desiredLoadBias < 0 || desiredLoadBias > 1) {
#ifdef IO_ON
		cerr << "In DP Load the desired load bias must be between 0 and 1."
				<< endl;
#endif

		throw 1;
	}
	if (desiredLoadGain < 0) {
#ifdef IO_ON
		cerr << "In DP Load the desired load gain must be non-negative."
				<< endl;
#endif
		throw 1;
	}

	double ratedPower = 0;
	for (unsigned int i = 0; i < _thrusters->size(); i++)
		ratedPower += (*_thrusters)[i].getPowerRated();

	_desiredLoadBias = ratedPower*desiredLoadBias;

	Load();
}

DPLoad::~DPLoad() {
}

double DPLoad::getDesiredLoad() const{
	return _desiredLoadBias + _desiredLoadGain * max(getLoad(),0.);
}

double DPLoad::getLoad() const{
	double sum = 0;
	for (unsigned int i = 0; i < _thrusters->size(); i++)
		sum += (*_thrusters)[i].getPower();
	return sum;
}

