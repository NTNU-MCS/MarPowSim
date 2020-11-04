/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "EmergencyBattery.h"

EmergencyBattery::EmergencyBattery() : _powerAvailable(0.),_powerGenerated(0.),_powerGeneratedPrevious(0.) {
}

EmergencyBattery::~EmergencyBattery() {
}

void EmergencyBattery::updateStatus(double powerGenerated, double powerAvailable){
  _powerGeneratedPrevious = _powerGenerated;
  _powerGenerated = powerGenerated;
  _powerAvailable = powerAvailable;
}
double EmergencyBattery::getPowerAvailable() const{
  return _powerAvailable;
}
double EmergencyBattery::getPowerGenerated() const{
  return _powerGenerated;
}
/* Power generated at the previous update */
double EmergencyBattery::getPowerGeneratedPrevious() const{
  return _powerGeneratedPrevious;
}
