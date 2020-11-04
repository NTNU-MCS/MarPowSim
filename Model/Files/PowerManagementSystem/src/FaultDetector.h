/*
 * FaultDetector.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef FAULTDETECTOR_H_
#define FAULTDETECTOR_H_

#include "Fault.h"
#include "PowerPlant.h"

/** \brief Virtual class for fault detectors
 *
 * Class to be implemented by fault detectors
 */
class FaultDetector {
public:
	FaultDetector(const PowerPlant* p);
	virtual ~FaultDetector();
	/** Detect if a fault has happened during last update, if so
	 * a pointer to a Fault object is returned with the time when the fault was detected
	 * and the suggested power available signal per bus.
	 *
	 * @param time timestamp for the given time, to be used to timestamp when the fault occurred.
	 * @return NULL if no fault has occurred or a fault object if a fault have occurred.
	 */
	Fault* getFault(const double time);

protected:
	const PowerPlant* _p;
    /**
     * Calculates new power available, due to the occurrence of the fault
     *
     * @param[in] affectedBus true for the busses which are affected by the fault.
     * @return Array with length equal number of bus, is populated with power available signal
     */
    virtual vector<double> powerAvailableAfterFault(const vector<bool> &affectedBus);

    /**
     * Calculate the reduced power available due to fast load reduction.
     * @param[in] affectedBus true for the busses which are affected by the fault.
     */
    virtual vector<double> powerAvailableAfterFaultThrusterFlr(const double* powerAvailableBus, const vector<bool> &affectedBus);

    /**
     * Detects if a fault has occurred
     * @param[out] affectedBus true for the busses which are affected by the fault.
     * @return true if a fault is detected, false if not
     */
    virtual bool detect(vector<bool> &affectedBus) = 0;

};

#endif /* FAULTDETECTOR_H_ */
