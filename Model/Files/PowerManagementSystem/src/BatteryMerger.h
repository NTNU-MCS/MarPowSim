/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef BATTERYMERGER_H_
#define BATTERYMERGER_H_

#include <vector>
#include "EmergencyBattery.h"

class BatteryMerger {
public:
  BatteryMerger();
  virtual ~BatteryMerger();
    /// Add a battery set to the container
    void add(EmergencyBattery*);

    /// Get pointer to battery set i
    EmergencyBattery* get(const unsigned int i);
    /// Get pointer to generator set i
    const EmergencyBattery* get(const unsigned int i) const;

    /// Removes all emergency batteries sets from container
    void clear();

    /// number of emergency batteries
    unsigned long size() const;

    /// Total power available for all emergency batteries sets in container
    double getPowerAvailable() const;
    /// Total power generated for all emergency batteries sets in container
    double getPowerGenerated() const;
    /// Total power generated for all emergency batteries sets in container before previous update
    double getPowerGeneratedPrevious() const;

private:
    std::vector<EmergencyBattery*> _batteries;
};

#endif /* BATTERYMERGER_H_ */
