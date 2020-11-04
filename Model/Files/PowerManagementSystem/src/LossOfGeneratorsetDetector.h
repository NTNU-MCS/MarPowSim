/*
 * LossOfGeneratorsetDetector.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef LOSSOFGENERATORSETDETECTOR_H_
#define LOSSOFGENERATORSETDETECTOR_H_

#include "FaultDetector.h"

/** \brief Detector for loss of generatorSets*/
class LossOfGeneratorSetDetector: public FaultDetector {

public:
	LossOfGeneratorSetDetector(const PowerPlant* p);
	virtual ~LossOfGeneratorSetDetector();
	bool detect(vector<bool> &affectedBus);
private:

};

#endif /* LOSSOFGENERATORSETDETECTOR_H_ */
