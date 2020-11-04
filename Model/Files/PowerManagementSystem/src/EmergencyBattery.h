/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef EMERGENCYBATTERY_H_
#define EMERGENCYBATTERY_H_

class EmergencyBattery {
public:
  EmergencyBattery();
  virtual ~EmergencyBattery();
  void updateStatus(double powerGenerated, double powerAvailable);
  double getPowerAvailable() const;
  double getPowerGenerated() const;
  /* Power generated at the previous update */
  double getPowerGeneratedPrevious() const;
private:
  double _powerAvailable;
  double _powerGenerated;
  double _powerGeneratedPrevious;

};

#endif /* EMERGENCYBATTERY_H_ */
