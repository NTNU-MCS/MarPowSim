/*
 * PowerPlant.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "PowerPlant.h"
#include <limits>

PowerPlant::PowerPlant() :
		_firstUpdate(true), _busTieBreakerOpened(false) {
}

PowerPlant::PowerPlant(PowerPlant const & po) :
		_firstUpdate(po._firstUpdate), _switchboard2bus(po._switchboard2bus), _busTieBreakerOpened(
				po._busTieBreakerOpened), _switchboard(po._switchboard) {
	setBus(po.getNumberOfBus());
}

PowerPlant & PowerPlant::operator=(PowerPlant const & other) {
	_firstUpdate = other._firstUpdate;
	_switchboard2bus = other._switchboard2bus;
	_busTieBreakerOpened = other._busTieBreakerOpened;
	_switchboard = other._switchboard;
	setBus(other.getNumberOfBus());
	return *this;
}

PowerPlant::~PowerPlant() {
}

void PowerPlant::addSwitchboard(const Switchboard& sIn) {
	_switchboard.push_back(sIn);
}

void PowerPlant::setBus(unsigned int nbus) {
	// clear and resize _bus vector
	for (unsigned int i = 0; i < _bus.size(); i++)
		_bus[i].clear();
	_bus.resize(nbus);
	// Assign switchboard to bus
	for (unsigned int i = 0; i < _switchboard2bus.size(); i++) {
		if (_switchboard2bus[i] >= nbus)
			throw 1;
		
		_bus[_switchboard2bus[i]].addSwitchboard(&_switchboard[i]);
	}
}

void PowerPlant::updateSwitchboard2Bus(unsigned int nbus,
		const vector<unsigned int> &switchboard2Bus) {
	_busTieBreakerOpened = false;
	for (unsigned i = 0; i < getNumberOfBus(); i++){
		getBus(i)->setHasBusTieBreakerOpened(false);
	}
	
	if (_firstUpdate)
		_firstUpdate = false;
	else if (switchboard2Bus == _switchboard2bus)
		return;
	else if (nbus >= getNumberOfBus())
		_busTieBreakerOpened = true;
	const vector<unsigned int> oldSwitchboard2Bus = _switchboard2bus;
	_switchboard2bus = switchboard2Bus;

	// clear and resize _bus vector
	setBus(nbus);

	// Set which buses are affected by a disconnection
	if (_busTieBreakerOpened){
		vector<long> old2new(oldSwitchboard2Bus.size(),-1);
		for (unsigned int swb = 0; swb < getNumberOfSwitchboards();swb++){
			if (old2new[oldSwitchboard2Bus[swb]] == -1)
				old2new[oldSwitchboard2Bus[swb]] = switchboard2Bus[swb];
			else if (old2new[oldSwitchboard2Bus[swb]] != switchboard2Bus[swb]){
				getBus(old2new[oldSwitchboard2Bus[swb]])->setHasBusTieBreakerOpened(true);
				getBus(switchboard2Bus[swb])->setHasBusTieBreakerOpened(true);
			}
		}
	}
}

unsigned int PowerPlant::getNumberOfBus() const {
	return (int) _bus.size();
}

unsigned long PowerPlant::getNumberOfSwitchboards() const {
	return _switchboard.size();
}

Bus* PowerPlant::getBus(unsigned int i) {
	return &_bus[i];
}

const Bus* PowerPlant::getBus(unsigned int i) const {
	return &_bus[i];
}

unsigned long PowerPlant::getNumberOfGeneratorset() const {
	unsigned long sum = 0;
	for (unsigned int i = 0; i < getNumberOfSwitchboards(); i++)
		sum += getSwitchboard(i)->getNumberOfGeneratorSets();
	return sum;
}

unsigned long PowerPlant::getNumberOfEmergencyBatteries() const {
  unsigned long sum = 0;
  for (unsigned int i = 0; i < getNumberOfSwitchboards(); i++)
      sum += getSwitchboard(i)->getNEmergencyBatteries();
  return sum;
}

Switchboard* PowerPlant::getSwitchboard(unsigned long i) {
	return &_switchboard[i];
}

const Switchboard* PowerPlant::getSwitchboard(unsigned long i) const {
	return &_switchboard[i];
}

GeneratorSet* PowerPlant::getGeneratorset(unsigned long i) {
	for (unsigned long swb = 0; swb < getNumberOfSwitchboards(); swb++) {
		if (i >= getSwitchboard(swb)->getNumberOfGeneratorSets())
			i -= getSwitchboard(swb)->getNumberOfGeneratorSets();
		else
			return getSwitchboard(swb)->getGeneratorSet(i);
	}

	throw 0;
}

const GeneratorSet* PowerPlant::getGeneratorset(unsigned long i) const {
	for (unsigned long swb = 0; swb < getNumberOfSwitchboards(); swb++) {
		if (i >= getSwitchboard(swb)->getNumberOfGeneratorSets())
			i -= getSwitchboard(swb)->getNumberOfGeneratorSets();
		else
			return getSwitchboard(swb)->getGeneratorSet(i);
	}

	throw 0;
}

void PowerPlant::updateGeneratorsets(const double* powerGenerated,
		const double* powerAvailable, const bool* connected) {
	unsigned int genNr = 0;
	for (unsigned int j = 0; j < getNumberOfSwitchboards(); j++) {
		Switchboard* s = getSwitchboard(j);
		for (unsigned int i = 0; i < s->getNumberOfGeneratorSets(); i++) {
			GeneratorSet* g = s->getGeneratorSet(i);
			g->updateStatus(connected[genNr], powerGenerated[genNr],
					powerAvailable[genNr]);
			genNr++;
		}
	}
}

void PowerPlant::updateEmergencyBatteries(const double* powerDelivered, const double* powerAvailable){
  unsigned int batNr = 0;
    for (unsigned int j = 0; j < getNumberOfSwitchboards(); j++) {
        Switchboard* s = getSwitchboard(j);
        for (unsigned int i = 0; i < s->getNEmergencyBatteries(); i++) {
            EmergencyBattery* b = s->getEmergencyBattery(i);
            b->updateStatus(powerDelivered[batNr],
                powerAvailable[batNr]);
            batNr++;
        }
    }
}

void PowerPlant::updateLoads(const double* powerHighPriority,
		const double* powerDesiredHighPriority, const double* powerLowPriority,
		const double* powerDesiredLowPriority) {
	for (unsigned long i = 0; i < getNumberOfSwitchboards(); i++) {
		Switchboard* s = getSwitchboard(i);
		s->getLoadHighPriority()->setLoad(powerHighPriority[i]);
		s->getLoadHighPriority()->setDesiredLoad(powerDesiredHighPriority[i]);
		s->getLoadLowPriority()->setLoad(powerLowPriority[i]);
		s->getLoadLowPriority()->setDesiredLoad(powerDesiredLowPriority[i]);
	}
}

unsigned long PowerPlant::getNumberOfThrusters() const {
	unsigned long sum = 0;
	for (unsigned int i = 0; i < getNumberOfSwitchboards(); i++)
		sum += getSwitchboard(i)->getNThrusters();
	return sum;
}

vector<double> PowerPlant::getPowerThrusters() const {
	vector<double> r(getNumberOfThrusters());
	for (unsigned int i = 0; i < getNumberOfThrusters(); i++)
		r[i] = getThruster(i)->getPower();
	return r;
}

void PowerPlant::updateThrusters(const double* power) {
	for (unsigned int i = 0; i < getNumberOfThrusters(); i++)
		getThruster(i)->setPower(power[i]);
}

bool PowerPlant::hasBusTieBreakerOpened() const {
	return _busTieBreakerOpened;
}

Thruster* PowerPlant::getThruster(unsigned long i) {
	for (unsigned int swb = 0; swb < getNumberOfSwitchboards(); swb++) {
		if (i >= getSwitchboard(swb)->getNThrusters())
			i -= getSwitchboard(swb)->getNThrusters();
		else
			return getSwitchboard(swb)->getThruster(i);
	}
	throw 0;
}

const Thruster* PowerPlant::getThruster(unsigned long i) const {
	for (unsigned long swb = 0; swb < getNumberOfSwitchboards(); swb++) {
		if (i >= getSwitchboard(swb)->getNThrusters())
			i -= getSwitchboard(swb)->getNThrusters();
		else
			return getSwitchboard(swb)->getThruster(i);
	}
	throw 0;
}
