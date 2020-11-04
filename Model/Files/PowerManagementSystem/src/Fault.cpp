/*
 * Fault.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 *
 *   \brief Class for storing details about a fault
 */

#include "Fault.h"
#ifdef IO_ON
#include <iostream>
#endif

Fault::Fault(const double time, vector<double> &powerAvailable,
		vector<double> &powerThrusters,
		vector<double> powerAvailableThrusterFastLoadReduction) :
		_time(time), _powerAvailable(powerAvailable), _powerThrustersPreFault(
				powerThrusters), _powerAvailableThrusterFastLoadReduction(
				powerAvailableThrusterFastLoadReduction) {
#ifdef IO_ON
	std::cout << "New fault detected after: " << time << " seconds"<< std::endl;
#endif
}

Fault::~Fault() {
}

double Fault::getTime() const{
	return _time;
}

vector<double> Fault::getPowerAvailable() const{
	return _powerAvailable;
}

vector<double> Fault::getPowerThrustersPreFault() const{
	return _powerThrustersPreFault;
}

vector<double> Fault::getPowerAvailableThrusterFastLoadReduction() const{
	return _powerAvailableThrusterFastLoadReduction;
}
