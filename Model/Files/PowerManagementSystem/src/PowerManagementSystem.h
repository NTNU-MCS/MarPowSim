/*
 * PowerManagementSystem.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef POWERMANAGEMENTSYSTEM_H_
#define POWERMANAGEMENTSYSTEM_H_

#include "Fault.h"
#include <vector>
#include "PowerPlant.h"
#include "PowerAvailable.h"
#include "FaultDetector.h"
#include "FastLoadReduction.h"
#include "FaultList.h"
#include <list>

using namespace std;

/** \brief Power Management System for calculation of Power Available and Fast Load Reduction.
 *
 * Calculates power available for loads, by current power available of generator sets.
 * This is the allocated to high priority loads, dp loads, and low priority loads (in prioritized order).
 *
 * Fault detection system is also implemented to detect trip of generator breaker and bus tie breaker.
 * In such cases the fast load reduction (FLR) will reduce the thruster loads.
 */
class PowerManagementSystem {
public:
	PowerManagementSystem(const PowerPlant &p,///< Power plant (a deep copy is done)
			double reactionTimePA,///< Reaction time for power available
			double ramptimePA,///< Ramp time for power available
			double reactionTimeFLR,///< Reaction time Fast Load Reduction
			double rampTimeFLR,///< Ramp time for power available
			bool faultDetectionEnabled ///< Enable fault detection
			);
	virtual ~PowerManagementSystem();

	/** Updates data for power management system
	 */
	void update(const double* generatorsetPowerDelivered, ///< Delivered power of generator sets
			const double* generatorsetPowerAvailable, ///< Available power from generator sets
			const bool* generatorsetConnected, ///< Connected generator sets
			const double* emergencyBatteriesPowerDeliverd, ///< Delivered power of emergency batteries
			const double* emergencyBatteriesPowerAvailable, ///< Available power of emergency batteries
			const double* powerHighPriority, ///< Consumed power by high priority loads
			const double* powerDesiredHighPriority, ///< Desired power by high priority loads
			const double* powerLowPriority, ///< Consumed power by low priority loads
			const double* powerDesiredLowPriority, ///< Desired power by low priority loads
			unsigned int nbus, ///< Number of busses
			const vector<unsigned int> &switchboard2Bus, ///< Switchboard to bus id (zero-index)
			const double* thrusterPower, ///< Consumed power by thruster
			const double &time ///< Current time in seconds
			);

    /** Updates data for power management system without batteries
     */
    void update(const double* generatorsetPowerDelivered, ///< Delivered power of generator sets
            const double* generatorsetPowerAvailable, ///< Available power from generator sets
            const bool* generatorsetConnected, ///< Connected generator sets
            const double* powerHighPriority, ///< Consumed power by high priority loads
            const double* powerDesiredHighPriority, ///< Desired power by high priority loads
            const double* powerLowPriority, ///< Consumed power by low priority loads
            const double* powerDesiredLowPriority, ///< Desired power by low priority loads
            unsigned int nbus, ///< Number of busses
            const vector<unsigned int> &switchboard2Bus, ///< Switchboard to bus id (zero-index)
            const double* thrusterPower, ///< Consumed power by thruster
            const double &time ///< Current time in seconds
            );

	const PowerPlant* getPowerPlant() const {return &_powerPlant;};
private:

	PowerPlant _powerPlant;
	vector<FaultDetector*> _faultDetector;
	FaultList _faults;
	PowerAvailable* _powerAvailable;
	FastLoadReduction* _flr;
	double _lastUpdated;

	void update(const double &time);

    /** Detects faults
     * @param pointer to integer with number of faults, populated during call
     * @return Faults occurred during last update
     */
    void detectFaults(const double &time);
};

#endif /* POWERMANAGEMENTSYSTEM_H_ */
