/*
 * PowerAvailableCalculator.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef POWERAVAILABLECALCULATOR_H_
#define POWERAVAILABLECALCULATOR_H_

#include <list>
#include "Fault.h"
#include "PowerPlant.h"
#include "FaultList.h"

using namespace std;

/** \brief Class to calculate power available and set it at consumers
 *
 *  Uses power available signal from generator set to calculate total power available on each bus.
 *  This is then allocated by priority to high priority loads, DPLoad, and low priority loads.
 *
 *  If faults has been detected the power available can be reduced by their recovery strategy.
 * */
class PowerAvailable {
public:
	PowerAvailable(const FaultList* faults, PowerPlant* _powerPlant,double reactionTime, double rampTime);
	virtual ~PowerAvailable();

	/// Calculates and update power available
	void update(const double &time);

	/// Calculates power available, given faults and/or current system state
	/// @return Total available power for bus
	vector<double> calculatePowerAvailableBus(const double &time);

	/// Distributes and set power available in power plant
	void setPowerAvailableLoads(vector<double> powerAvailable);
private:
	const FaultList* _faults;
	PowerPlant* _powerPlant;
	double _reactionTime;
	double _rampTime;

	/// Set power available for the given load
	/// @return remaining power available
	template<class T> double setPowerAvailablePartLoad(const double excessCapacity,
        const unsigned long &nSwitchboard, Bus* b, double powerAvailable,
        LoadMerger<T>* load);
};

#endif /* POWERAVAILABLECALCULATOR_H_ */
