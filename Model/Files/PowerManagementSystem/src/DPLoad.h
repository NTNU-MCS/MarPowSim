/*
 * Load.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef DPLOAD_H_
#define DPLOAD_H_

#include <vector>
#include "Load.h"
#include "Thruster.h"

using namespace std;

/** \brief Group together thrusters for common calculations
 *
 *
 */
class DPLoad: public Load {
public:
	DPLoad(const vector<Thruster> *thrusters,///< Vector of thrusters connected to the same switchboard
			double desiredPowerAvailableBias, ///< Desired absolute extra power available in percentage of rated power of connected thrusters
			double desiredPowerAvailableGain ///< Desired relative extra power available, relative to used power.
			);
	virtual ~DPLoad();

	/// Get sum of consumed power of thrusters
	virtual double getLoad() const;
	/// Desired load is set to desiredPowerAvailableBias + desiredPowerAvailableGain*getLoad()
	virtual double getDesiredLoad() const;
private:
	virtual void setLoad(double load) {
		throw 0;
	}

	virtual void setDesiredLoad(double load) {
		throw 0;
	}

	const vector<Thruster>* _thrusters;
	double _desiredLoadBias;
	double _desiredLoadGain;
};

#endif /* LOAD_H_ */
