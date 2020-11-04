/*
 * Switchboard.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef SWITCHBOARD_H_
#define SWITCHBOARD_H_
#include "GeneratorSet.h"
#include <vector>
#include "Load.h"
#include "Thruster.h"
#include "DPLoad.h"
#include "EmergencyBattery.h"

using namespace std;

/** \brief Class for physical switchboards
 *
 * Loads, Thrusters, and GeneratorSets are connected to switchboards. Multiple switchboards can be connected by bus tie breakers to a bus.
 *
 */
class Switchboard {
public:

    Switchboard(const unsigned long numberOfGeneratorSets,
            const double * ratedPowerOfGeneratorsets,
            const unsigned long numberOfThrusters,
            const double * ratedPowerOfThrusters,
            double desiredPowerAvailableBias, ///< Desired absolute extra power available for DP in percentage of rated power of connected thrusters
            double desiredPowerAvailableGain ///< Desired relative extra power available for DP, relative to used power.
            );
    Switchboard(const unsigned long numberOfGeneratorSets,
            const double * ratedPowerOfGeneratorsets,
            const unsigned long numberOfThrusters,
            const double * ratedPowerOfThrusters,
            const unsigned long numberOfEmergencyBatteries,
            double desiredPowerAvailableBias, ///< Desired absolute extra power available for DP in percentage of rated power of connected thrusters
            double desiredPowerAvailableGain ///< Desired relative extra power available for DP, relative to used power.
            );
	Switchboard(const Switchboard & other);
	Switchboard& operator=(const Switchboard & other);
	virtual ~Switchboard();
	unsigned long getNThrusters() const;
	unsigned long getNEmergencyBatteries() const;
	unsigned long getNumberOfGeneratorSets() const;
	GeneratorSet* getGeneratorSet(unsigned long);
	const GeneratorSet* getGeneratorSet(unsigned long) const;
	Thruster* getThruster(unsigned long);
	const Thruster* getThruster(unsigned long) const;
    EmergencyBattery* getEmergencyBattery(unsigned long);
    const EmergencyBattery* getEmergencyBattery(unsigned long) const;
	Load* getLoadLowPriority();
	const Load* getLoadLowPriority() const;
	Load* getLoadHighPriority();
	const Load* getLoadHighPriority() const;
	DPLoad* getLoadDP();
	const DPLoad* getLoadDP() const;
private:

	vector<GeneratorSet> _generatorSets;
	vector<Thruster> _thrusters;
	vector<EmergencyBattery> _emergencyBattery;
	Load _load[2];
	DPLoad _dpLoad;
	double _desiredPowerAvailableBias;
	double _desiredPowerAvailableGain;
};

#endif /* SWITCHBOARD_H_ */
