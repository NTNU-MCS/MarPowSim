/*
 * FastLoadReduction.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	Jun 25, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef FASTLOADREDUCTION_H_
#define FASTLOADREDUCTION_H_

#include "PowerPlant.h"
#include "Fault.h"
#include "FaultList.h"
#include <list>

/** \brief Reduce power available on thrusters after fault
 *
 * After a fault has occurs the system waits the time specified in reaction time before it do any action.
 * The power available on each thruster is then reduced to the level specified in fault.getPowerAvailableThrusterFastLoadReduction().
 * This is the linearly increased back to rated value of thruster after ramp time full.
 */
class FastLoadReduction {
public:
	FastLoadReduction(double reactionTime,///< Time between the fault is detected and FLR reacts
			double rampTimeFull,///< Time from FLR reacts till the power reduction is removed
			PowerPlant* powerPlant, ///< Ptr to power plant which should be modified after fault
			const FaultList* faults ///< Ptr to ptr list of faults, faults should be added to this list by fault detectors
			);
	virtual ~FastLoadReduction();
	void update(const double &time);

	/// Calculates power available, given faults and/or current system state
	/// @param time Current simulation time
	/// @param[out] powerAvailable Total available power for each thruster
	void calculatePowerAvailable(const double &time, double* powerAvailable);

private:
	double _restoreTime; // Time from FLR is started till power available is back to pre-fault power value
	double _rampTimeFull; // Ramp time (0%-100%) for increasing power available from pre-fault power value to rated value
	PowerPlant* _p;
	const FaultList* _faults;
};

#endif /* FASTLOADREDUCTION_H_ */
