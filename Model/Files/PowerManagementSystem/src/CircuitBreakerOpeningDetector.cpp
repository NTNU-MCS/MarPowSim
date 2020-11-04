/*
 * CircuitBreakerOpeningDetector.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "CircuitBreakerOpeningDetector.h"

CircuitBreakerOpeningDetector::CircuitBreakerOpeningDetector(const PowerPlant* p) : FaultDetector(p) {
	// TODO Auto-generated constructor stub
	
}

CircuitBreakerOpeningDetector::~CircuitBreakerOpeningDetector() {
	// TODO Auto-generated destructor stub
}

bool CircuitBreakerOpeningDetector::detect(vector<bool> &affectedBus) {
	if (_p->hasBusTieBreakerOpened()){
		for (unsigned int i = 0; i< affectedBus.size();i++)
			affectedBus[i] = _p->getBus(i)->hasBusTieBreakerOpened();
		return true;
	} else {
		return false;
	}
}
