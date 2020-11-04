/*
 * FastLoadReduction.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	Jun 25, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "FastLoadReduction.h"

FastLoadReduction::FastLoadReduction(double restoreTime, double rampTimeFull,
		PowerPlant* powerPlant, const FaultList* faults) : _restoreTime(restoreTime), _rampTimeFull(rampTimeFull), _p(powerPlant), _faults(faults) {


}

FastLoadReduction::~FastLoadReduction() {
}

void FastLoadReduction::update(const double &time) {
	unsigned long nThrusters = _p->getNumberOfThrusters();
	vector<double> pa(nThrusters,0.);
	calculatePowerAvailable(time,&pa[0]);
	for (unsigned long i = 0; i < nThrusters;i++)
		_p->getThruster(i)->setFLRPowerAvailable(pa[i]);

}


void  FastLoadReduction::calculatePowerAvailable(const double &time, double* pa) {
	unsigned long nThrusters = _p->getNumberOfThrusters();

	if(nThrusters == 0)
		return;

	for (unsigned int i = 0; i < nThrusters; i++) {
		pa[i] = _p->getThruster(i)->getPowerRated();
	}

	for (FaultList::const_iterator f = _faults->begin(); f != _faults->end();
			++f) {
	  const Fault* fault = *f;
		const double dt = time - fault->getTime();
		if (dt >= _restoreTime && dt < (_restoreTime + _rampTimeFull)) {
			double available = (dt - _restoreTime) / _rampTimeFull;
			for (unsigned long i = 0; i < nThrusters; i++) {
				double pReduced = fault->getPowerAvailableThrusterFastLoadReduction()[i];
				double pRated = _p->getThruster(i)->getPowerRated();
				double paCandidate = pReduced
						+ (pRated - pReduced) * available;
				if (pa[i] > paCandidate)
					pa[i] = paCandidate;
			}
		}
	}
	return;
}
