/*
 * Thruster.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef THRUSTER_H_
#define THRUSTER_H_

/** \brief Class for Thruster
 *
 * Note that DP loads should be used to set power available
 */
class Thruster {
public:
	Thruster(const double &ratedPower);
	virtual ~Thruster();
	double getPower() const;
	double getPowerAvailable() const;
	double getPowerRated() const;
	void setPower(double power);
	/// Set fast load reduction power available, for power available use DPLoad
	void setFLRPowerAvailable(double);
private:
	double _load;
	double _powerAvailable;
	double _ratedPower;
};

#endif /* THRUSTER_H_ */
