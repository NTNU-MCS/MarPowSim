/*
 * PowerAvailableCalculator.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "PowerAvailable.h"

PowerAvailable::PowerAvailable(const FaultList* faults, PowerPlant* powerPlant,
		double reactionTime, double rampTime) :
		_faults(faults), _powerPlant(powerPlant), _reactionTime(reactionTime), _rampTime(
				rampTime) {
	
}

PowerAvailable::~PowerAvailable() {
	// TODO Auto-generated destructor stub
}

vector<double> PowerAvailable::calculatePowerAvailableBus(const double &time) {
	unsigned short nBus = _powerPlant->getNumberOfBus();
	vector<double> pa(nBus);
	vector<double> ratedPower(nBus);
	for (unsigned int i = 0; i < nBus; i++) {
		pa[i] =
				_powerPlant->getBus(i)->getPowerAvailable();
		ratedPower[i] =
				_powerPlant->getBus(i)->getPowerRated();
	}
	double* paFramped = new double[nBus];
	for (FaultList::const_iterator f = _faults->begin(); f != _faults->end();
			++f) {
		const vector<double> paF = (*f)->getPowerAvailable();
		double dt = time - ((*f)->getTime() + _reactionTime);
		if (dt >= 0) {
			for (unsigned int j = 0; j < nBus; j++) {
				paFramped[j] = paF[j] + ratedPower[j] * dt / _rampTime;
				if (pa[j] > paFramped[j])
					pa[j] = paFramped[j];
			}
		}
	}
	delete[] paFramped;
	return pa;
}



void PowerAvailable::update(const double& time) {
	vector<double> powerAvailable = calculatePowerAvailableBus(time);
	setPowerAvailableLoads(powerAvailable);
}

void PowerAvailable::setPowerAvailableLoads(vector<double> powerAvailable) {
	/// Iterates through each bus and set power available on each load on the bus.
	for (unsigned int i = 0; i < _powerPlant->getNumberOfBus(); i++) {
		Bus* b = _powerPlant->getBus(i);
		unsigned long nSwitchboard = b->getNumberOfSwitchboard();

		// Calculates excess capacity
		double desiredTotal = b->getDesiredPower();
		double excessCapacity = 0;
		if (desiredTotal < powerAvailable[i])
			excessCapacity = powerAvailable[i] - desiredTotal;

		// Distributes to high priority loads:
		powerAvailable[i] = setPowerAvailablePartLoad(excessCapacity / 3,
				nSwitchboard, b, powerAvailable[i], b->getLoadHighPriority());
		powerAvailable[i] = setPowerAvailablePartLoad(excessCapacity / 3,
				nSwitchboard, b, powerAvailable[i], b->getLoadDP());
		powerAvailable[i] = setPowerAvailablePartLoad(excessCapacity / 3,
				nSwitchboard, b, powerAvailable[i], b->getLoadLowPriority());

	}

}

/// Calculates reduction
/// Returns total remaining available power
template<class T> double PowerAvailable::setPowerAvailablePartLoad(
		const double excessCapacity, const unsigned long &nSwitchboard, Bus* b,
		double powerAvailable, LoadMerger<T>* load) {

	// Ratio between power available and desired
	double powerAvailableMaintainedRatio;
	// Desired power available
	double powerDesired = load->getDesiredLoad();
	if (powerAvailable <= 0) {
		// No available power
		powerAvailable = 0;
		powerAvailableMaintainedRatio = 0;
	} else {
		if (powerDesired > powerAvailable && powerDesired > 0) {
			// Not enough available power
			powerAvailableMaintainedRatio = powerAvailable / powerDesired;

		} else {
			// Enough power available
			powerAvailableMaintainedRatio = 1;

		}
	}
	
	// Sets
	for (unsigned int j = 0; j < nSwitchboard; j++) {
		T* l = load->getPartLoad(j);
		double powerAvailableToLoad = powerAvailableMaintainedRatio
				* l->getDesiredLoad() + excessCapacity / nSwitchboard;
		l->setPowerAvailable(powerAvailableToLoad);
		powerAvailable -= powerAvailableToLoad;
	}

	return powerAvailable;

}

