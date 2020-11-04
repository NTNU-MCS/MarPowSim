/*
 * CircuitBreakerOpeningDetector.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef CIRCUITBREAKEROPENINGDETECTOR_H_
#define CIRCUITBREAKEROPENINGDETECTOR_H_

#include "FaultDetector.h"

/// Detects if a bus-tie breaker has opened
class CircuitBreakerOpeningDetector: public FaultDetector {
public:
	CircuitBreakerOpeningDetector(const PowerPlant* p);
	virtual ~CircuitBreakerOpeningDetector();
	bool detect(vector<bool> &affectedBus);

};

#endif /* CIRCUITBREAKEROPENINGDETECTOR_H_ */
