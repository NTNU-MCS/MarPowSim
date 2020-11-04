/*
 * ThrusterMerger.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef THRUSTERMERGER_H_
#define THRUSTERMERGER_H_

#include <vector>
#include "Thruster.h"

using namespace std;

/** \brief Class for virtually group Thruster */
class ThrusterMerger {
public:
	ThrusterMerger();
	virtual ~ThrusterMerger();
	void addThruster(Thruster*);
	unsigned long size() const;
	void clear();
	double getPowerAvailable() const;
	double getPower() const;
	Thruster* getThruster(const unsigned long i);
	const Thruster* getThruster(const unsigned long i) const;
private:
	vector<Thruster*> _thruster;
};

#endif /* THRUSTERMERGER_H_ */
