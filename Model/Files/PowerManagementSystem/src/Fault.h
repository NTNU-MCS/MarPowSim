/*
 * Fault.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef FAULT_H_
#define FAULT_H_

#include <vector>
using namespace std;

/** \brief Class for storing faults and how it should be recovered
 *
 */
class Fault {
public:
	Fault(const double time, ///< Time for when the fault was detected
	    vector<double> &powerAvailable, ///< Power available of each bus right after fault
	    vector<double> &powerThrusters, ///< Consumed power right before fault
		vector<double> powerAvailableThrusterFastLoadReduction ///< Power available of thruster right after Fast Load reduction per thruster
			);
	virtual ~Fault();
	double getTime() const;
	vector<double> getPowerAvailable() const; // Power available of each bus right after fault
	vector<double> getPowerThrustersPreFault() const; // Consumed power pre-fault
	vector<double> getPowerAvailableThrusterFastLoadReduction() const; // Power available of thruster right after Fast Load reduction per thruster
private:
	double _time;
	vector<double> _powerAvailable;
	vector<double> _powerThrustersPreFault;
	vector<double> _powerAvailableThrusterFastLoadReduction;
};

#endif /* FAULT_H_ */
