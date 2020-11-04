/*
 * Switchboard.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "Switchboard.h"

Switchboard::Switchboard(const unsigned long numberOfGeneratorSets,
		const double * ratedPowerOfGeneratorsets,
		const unsigned long numberOfThrusters,
		const double * ratedPowerOfThrusters,
		const unsigned long numberOfEmergencyBatteries,
		double desiredPowerAvailableBias,
		double desiredPowerAvailableGain) :
		_dpLoad(&_thrusters, desiredPowerAvailableBias,
				desiredPowerAvailableGain), _desiredPowerAvailableBias(
				desiredPowerAvailableBias), _desiredPowerAvailableGain(
				desiredPowerAvailableGain) {
	_generatorSets.reserve(numberOfGeneratorSets);
	for (unsigned long i = 0; i < numberOfGeneratorSets; i++)
		_generatorSets.push_back(GeneratorSet(ratedPowerOfGeneratorsets[i]));

	_thrusters.reserve(numberOfThrusters);
	for (unsigned long i = 0; i < numberOfThrusters; i++)
		_thrusters.push_back(Thruster(ratedPowerOfThrusters[i]));


	_emergencyBattery.reserve(numberOfEmergencyBatteries);
	for (unsigned long i = 0; i < numberOfEmergencyBatteries; i++)
	    _emergencyBattery.push_back(EmergencyBattery());
}

Switchboard::Switchboard(const unsigned long numberOfGeneratorSets,
        const double * ratedPowerOfGeneratorsets,
        const unsigned long numberOfThrusters,
        const double * ratedPowerOfThrusters,
        double desiredPowerAvailableBias,
        double desiredPowerAvailableGain) : _dpLoad(&_thrusters, desiredPowerAvailableBias,
            desiredPowerAvailableGain), _desiredPowerAvailableBias(
            desiredPowerAvailableBias), _desiredPowerAvailableGain(
            desiredPowerAvailableGain) {
  _generatorSets.reserve(numberOfGeneratorSets);
  for (unsigned long i = 0; i < numberOfGeneratorSets; i++)
      _generatorSets.push_back(GeneratorSet(ratedPowerOfGeneratorsets[i]));

  _thrusters.reserve(numberOfThrusters);
  for (unsigned long i = 0; i < numberOfThrusters; i++)
      _thrusters.push_back(Thruster(ratedPowerOfThrusters[i]));


  _emergencyBattery.clear();
}

Switchboard::Switchboard(const Switchboard & other) :
		_generatorSets(other._generatorSets), _thrusters(other._thrusters),
		_dpLoad(&(this->_thrusters),other._desiredPowerAvailableBias,
				other._desiredPowerAvailableGain), _desiredPowerAvailableBias(
				other._desiredPowerAvailableBias), _desiredPowerAvailableGain(
				other._desiredPowerAvailableGain),_emergencyBattery(other._emergencyBattery) {
            _load[0] = other._load[0];
            _load[1] = other._load[1];

}

Switchboard& Switchboard::operator=(const Switchboard & other) {
	_generatorSets = other._generatorSets;
	_thrusters = other._thrusters;
	_emergencyBattery = other._emergencyBattery;
	_load[0] = other._load[0];
	_load[1] = other._load[1];
	_desiredPowerAvailableBias = other._desiredPowerAvailableBias;
	_desiredPowerAvailableGain = other._desiredPowerAvailableGain;
	_dpLoad = DPLoad(&(this->_thrusters), other._desiredPowerAvailableBias,
			other._desiredPowerAvailableGain);

	return *this;
}

Switchboard::~Switchboard() {
// TODO Auto-generated destructor stub
}

unsigned long Switchboard::getNumberOfGeneratorSets() const {
	return _generatorSets.size();
}

GeneratorSet* Switchboard::getGeneratorSet(unsigned long i) {
	return &_generatorSets[i];
}

const GeneratorSet* Switchboard::getGeneratorSet(unsigned long i) const {
	return &_generatorSets[i];
}

Load* Switchboard::getLoadLowPriority() {
	return &_load[0];
}

const Load* Switchboard::getLoadLowPriority() const{
    return &_load[0];
}

Load* Switchboard::getLoadHighPriority() {
	return &_load[1];
}

const Load* Switchboard::getLoadHighPriority() const{
    return &_load[1];
}

unsigned long Switchboard::getNThrusters() const {
	return _thrusters.size();
}

Thruster* Switchboard::getThruster(unsigned long i) {
	return &_thrusters[i];
}

const Thruster* Switchboard::getThruster(unsigned long i) const {
	return &_thrusters[i];
}

DPLoad* Switchboard::getLoadDP() {
	return &_dpLoad;
}

const DPLoad* Switchboard::getLoadDP() const{
    return &_dpLoad;
}

unsigned long Switchboard::getNEmergencyBatteries() const{
  return _emergencyBattery.size();
}

EmergencyBattery* Switchboard::getEmergencyBattery(unsigned long i){
  return &_emergencyBattery[i];
}
const EmergencyBattery* Switchboard::getEmergencyBattery(unsigned long i) const{
  return &_emergencyBattery[i];
}
