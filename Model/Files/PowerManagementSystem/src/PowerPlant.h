/*
 * PowerPlant.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef POWERPLANT_H_
#define POWERPLANT_H_

#include <vector>
#include "Switchboard.h"
#include "Bus.h"

/** \brief Class containing all electrical components
 *
 */
class PowerPlant {
public:
	PowerPlant();
	PowerPlant(PowerPlant const & po);
	PowerPlant & operator=(PowerPlant const & other);
	virtual ~PowerPlant();
	/// Add switchboard to power plant, note that the power plant destructs the switchboard during destruction.
	void addSwitchboard(const Switchboard&);

	unsigned int getNumberOfBus() const;
	unsigned long getNumberOfSwitchboards() const;
	unsigned long getNumberOfGeneratorset() const;
	unsigned long getNumberOfThrusters() const;
	unsigned long getNumberOfEmergencyBatteries() const;
	GeneratorSet* getGeneratorset(unsigned long i);
	const GeneratorSet* getGeneratorset(unsigned long i) const;
	Thruster* getThruster(unsigned long i);
	const Thruster* getThruster(unsigned long i) const;
	Bus* getBus(unsigned int i);
	const Bus* getBus(unsigned int i) const;
	Switchboard* getSwitchboard(unsigned long i);
	const Switchboard* getSwitchboard(unsigned long i) const;
	void updateGeneratorsets(const double* powerDelivered,
			const double* powerAvailable, const bool* connected);
	void updateLoads(const double* powerHighPriority,
			const double* powerDesiredHighPriority,
			const double* powerLowPriority,
			const double* powerDesiredLowPriority);
	void updateSwitchboard2Bus(unsigned int nbus,
			const vector<unsigned int> &switchboard2Bus);
	void updateThrusters(const double* power);
	void updateEmergencyBatteries(const double* powerDelivered, const double* powerAvailable);
	vector<double> getPowerThrusters() const;

	/** Check if a bus tie breaker has opened.
	 *
	 * Detects if a bus tie breaker has opened.
	 * NB: This function is not valid if more breakers closes then opens.
	 *
	 * @return True if one breaker has opened, else false.
	 */
	bool hasBusTieBreakerOpened() const;
private:
	bool _firstUpdate;
	vector<unsigned int> _switchboard2bus;
	bool _busTieBreakerOpened;
	vector<Bus> _bus;
	vector<Switchboard> _switchboard;

	void setBus(unsigned int nbus);
};

#endif /* POWERPLANT_H_ */
