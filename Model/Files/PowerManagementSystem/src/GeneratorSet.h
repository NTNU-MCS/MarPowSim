/*
 * GeneratorSet.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef GENERATORSET_H_
#define GENERATORSET_H_

/** \brief Class for generator sets
 *
 */
class GeneratorSet {
public:
	GeneratorSet(const double ratedPower);
	virtual ~GeneratorSet();
	void updateStatus(bool connected, double powerGenerated, double powerAvailable);
	bool isConnected() const;
	bool wasConnected() const;
	/** @return true if it was connected at previous step but is not now */
	bool hasDisconnected() const;
	double getPowerAvailable() const;
	double getPowerGenerated() const;
	/* Power generated at the previous update */
	double getPowerGeneratedPrevious() const;
	double getPowerRated() const;
private:
	bool _connected;
	bool _wasconnected;
	bool _firstUpdate;
	double _powerAvailable;
	double _powerGenerated;
	double _powerGeneratedPrevious;
	double _ratedPower;
};

#endif /* GENERATORSET_H_ */
