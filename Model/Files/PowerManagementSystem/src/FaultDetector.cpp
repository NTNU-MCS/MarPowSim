/*
 * FaultDetector.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "FaultDetector.h"
#include <limits>

FaultDetector::FaultDetector(const PowerPlant* p) :
		_p(p) {
}

FaultDetector::~FaultDetector() {
}

Fault* FaultDetector::getFault(const double time) {
	vector<bool> affectedBus(_p->getNumberOfBus());
	if (!detect(affectedBus)) {
		return 0;
	}

	vector<double> powerAvailable = powerAvailableAfterFault(affectedBus);
	vector<double> powerAvailableThrusters =
			powerAvailableAfterFaultThrusterFlr(&powerAvailable[0],
					affectedBus);
	vector<double> powerThruster = _p->getPowerThrusters();
	return new Fault(time, powerAvailable, powerThruster,
			powerAvailableThrusters);
}

vector<double> FaultDetector::powerAvailableAfterFault(
		const vector<bool> &affectedBus) {
	vector<double> powerAvailable(_p->getNumberOfBus());
	for (unsigned int i = 0; i < _p->getNumberOfBus(); i++) {
		if (affectedBus[i]) {
			powerAvailable[i] = 0;
			const Bus* bus = _p->getBus(i);
			unsigned long nGensets = bus->getNumberOfGeneratorSets();
			for (unsigned int j = 0; j < nGensets; j++) {
				const GeneratorSet* g = bus->getGeneratorSet(j);
				if (g->wasConnected() && g->isConnected()) {
					powerAvailable[i] += g->getPowerGeneratedPrevious();
				}

			}
			double consumedPower = bus->getConsumedPower();
			if (powerAvailable[i]>consumedPower){
				powerAvailable[i] = std::numeric_limits<double>::infinity();
			}
		} else
			powerAvailable[i] = std::numeric_limits<double>::infinity();
	}
	return powerAvailable;
}

vector<double> FaultDetector::powerAvailableAfterFaultThrusterFlr(
		const double* powerAvailableBus, const vector<bool> &affectedBus) {

	unsigned int thrusterNr = 0;
	vector<double> powerAvailable(_p->getNumberOfThrusters());
	unsigned int nBus = _p->getNumberOfBus();
	for (unsigned int i = 0; i < nBus; i++) {

		const Bus* bus = _p->getBus(i);
		double reductionRatio;

		if (affectedBus[i]) {
			double powerAvailableThrusters = powerAvailableBus[i]
					- bus->getLoadHighPriority()->getLoad()
					- bus->getLoadLowPriority()->getLoad();

			if (powerAvailableThrusters <= 0.) {
				powerAvailableThrusters = 0;
				reductionRatio = 0.;
			} else {
				reductionRatio = powerAvailableThrusters
						/ (bus->getThrusters()->getPower());
			}
		}

		unsigned long nThrusters = bus->getNumberOfThrusters();
		for (unsigned int j = 0; j < nThrusters; j++) {
			if (!affectedBus[i] || reductionRatio > 1)
				powerAvailable[thrusterNr++] =
						numeric_limits<double>::infinity();
			else
				powerAvailable[thrusterNr++] = bus->getThruster(j)->getPower()
						* reductionRatio;

		}

	}
	return powerAvailable;
}
