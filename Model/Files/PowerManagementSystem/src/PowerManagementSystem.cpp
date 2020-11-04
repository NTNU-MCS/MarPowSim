/*
 * PowerManagementSystem.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "PowerManagementSystem.h"
#include "CircuitBreakerOpeningDetector.h"
#include "LossOfGeneratorsetDetector.h"
#include "Fault.h"
#include <vector>
#include <float.h>
#include <algorithm>

PowerManagementSystem::PowerManagementSystem(const PowerPlant& p,
    double reactionTimePA,
    double ramptimePA,
    double restoreTimeFLR,
    double rampTimeFLR,
    bool faultDetectionEnabled) :
        _powerPlant(p) {
  _powerAvailable = new PowerAvailable(&_faults, &_powerPlant, reactionTimePA,
      ramptimePA);
  _flr = new FastLoadReduction(restoreTimeFLR, rampTimeFLR, &_powerPlant,
      &_faults);
  if (faultDetectionEnabled){
    _faultDetector.reserve(2);
    _faultDetector.push_back(new CircuitBreakerOpeningDetector(&_powerPlant));
    _faultDetector.push_back(new LossOfGeneratorSetDetector(&_powerPlant));
  }

  _faults.setRemoveTime(max(reactionTimePA + ramptimePA,
      restoreTimeFLR + rampTimeFLR));
  _lastUpdated = -DBL_MAX;
}

PowerManagementSystem::~PowerManagementSystem() {
  for (unsigned int i = 0; i < _faultDetector.size(); i++) {
    delete _faultDetector[i];
  }
  delete _powerAvailable;
  delete _flr;
}

void PowerManagementSystem::update(const double &time) {
  if (_lastUpdated < time) {
	  _faults.removeOldFaults(time);
    detectFaults(time);
    // TODO: detect change of switchboard to bus, and modify recorded faults thereafter

    _powerAvailable->update(time);
    _flr->update(time);
  }
  _lastUpdated = time;
}

void PowerManagementSystem::update(const double* generatorsetPowerDelivered,
    const double* generatorsetPowerAvailable,
    const bool* generatorsetConnected,
    const double* emergencyBatteriesPowerDelivered,
    const double* emergencyBatteriesPowerAvailable,
    const double* powerHighPriority,
    const double* powerDesiredHighPriority,
    const double* powerLowPriority,
    const double* powerDesiredLowPriority,
    unsigned int nbus,
    const vector<unsigned int> &switchboard2Bus,
    const double* thrusterPower,
    const double &time) {
  _powerPlant.updateGeneratorsets(generatorsetPowerDelivered,
      generatorsetPowerAvailable, generatorsetConnected);
  _powerPlant.updateLoads(powerHighPriority, powerDesiredHighPriority,
      powerLowPriority, powerDesiredLowPriority);
  _powerPlant.updateSwitchboard2Bus(nbus, switchboard2Bus);
  _powerPlant.updateThrusters(thrusterPower);
  _powerPlant.updateEmergencyBatteries(emergencyBatteriesPowerDelivered,emergencyBatteriesPowerAvailable);
  update(time);

}

void PowerManagementSystem::update(const double* generatorsetPowerDelivered,
    const double* generatorsetPowerAvailable,
    const bool* generatorsetConnected,
    const double* powerHighPriority,
    const double* powerDesiredHighPriority,
    const double* powerLowPriority,
    const double* powerDesiredLowPriority,
    unsigned int nbus,
    const vector<unsigned int> &switchboard2Bus,
    const double* thrusterPower,
    const double &time) {
  if (_powerPlant.getNumberOfEmergencyBatteries()==0){
    update(generatorsetPowerDelivered,
        generatorsetPowerAvailable,
        generatorsetConnected,
        0,
        0,
        powerHighPriority,
        powerDesiredHighPriority,
        powerLowPriority,
        powerDesiredLowPriority,
        nbus,
        switchboard2Bus,
        thrusterPower,
        time);
  } else {
// Should not be used if batteries are attached
    throw 0;
  }

}

void PowerManagementSystem::detectFaults(const double &time) {
  for (unsigned int i = 0; i < _faultDetector.size(); i++) {
    const Fault* f = _faultDetector[i]->getFault(time);
    if (f != 0) {
      _faults.push_front(f);
    }

  }

}
