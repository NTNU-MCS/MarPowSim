/*
 * Bus.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef BUS_H_
#define BUS_H_
#include <vector>
#include "Switchboard.h"
#include "MergeLoad.h"

#include "GeneratorsetMerger.h"
#include "ThrusterMerger.h"
#include "BatteryMerger.h"

/** \brief A virtual group of switchboards
 *
 * Used when multiple switchboards are connected together
 * and the calculation should be done as a group and not as individual switchboards.
 */
class Bus {
public:
	Bus();
	Bus(bool hasBusTieBreakerOpened ///< True if this bus is affected by an opening of a bus tie breaker during last iteration
			);
	virtual ~Bus();

	/** Add a pointer to switchboard to bus	 */
	void addSwitchboard(Switchboard*);
	Switchboard* getSwitchboard(unsigned int);

	/** Removes all switchboards from bus	 */
	void clear();
	unsigned long getNumberOfSwitchboard() const;
	unsigned long getNumberOfGeneratorSets() const;
	unsigned long getNumberOfThrusters() const;

	double getPowerAvailable() const;
	double getPowerRated() const;
	double getPowerGenerated() const;
	double getPowerPreviusGenerated() const;
    double getDesiredPower() const;
    double getConsumedPower() const;

    const GeneratorSet* getGeneratorSet(unsigned int) const;
    const Thruster* getThruster(unsigned int) const;
    const LoadMerger<Load>* getLoadLowPriority() const;
    const LoadMerger<DPLoad>* getLoadDP() const;
    const LoadMerger<Load>* getLoadHighPriority() const;
    const ThrusterMerger* getThrusters() const;
    const GeneratorSetMerger* getGeneratorsetMerged() const;

    LoadMerger<Load>* getLoadLowPriority();
    LoadMerger<Load>* getLoadHighPriority();
    LoadMerger<DPLoad>* getLoadDP();

	bool hasBusTieBreakerOpened() const {
		return _hasBusTieBreakerOpened;
	}
	;
	void setHasBusTieBreakerOpened(bool hasBusTieBreakerOpened) {
		_hasBusTieBreakerOpened = hasBusTieBreakerOpened;
	}
	;


private:
	bool _hasBusTieBreakerOpened;
	vector<Switchboard*> _switchboard;
	GeneratorSetMerger _generatorsets;
	ThrusterMerger _thrusters;
	BatteryMerger _batteries;
	LoadMerger<Load> _loadLP;
	LoadMerger<Load> _loadHP;
	LoadMerger<DPLoad> _loadDP;

    GeneratorSet* getGeneratorSet(unsigned int);
    Thruster* getThruster(unsigned int);

    ThrusterMerger* getThrusters();
    GeneratorSetMerger* getGeneratorsetMerged();

};

#endif /* BUS_H_ */
