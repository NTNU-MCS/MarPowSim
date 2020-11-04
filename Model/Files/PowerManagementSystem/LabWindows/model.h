/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */
#ifndef MODEL_h
# define MODEL_h 
#define MAX_WIDTH 100  
typedef struct {
  	double nSwb;                             
  	double nGeneratorSetPerSwb[MAX_WIDTH];
	double ratedPowerGenset[MAX_WIDTH];
    double nThrustersPerSwb[MAX_WIDTH];
	 double ratedPowerThrusters[MAX_WIDTH];
	   double dpPowerAvailableBias;
	 double dpPowerAvailableGain;
    double reactionTimePA;
    double ramptimePA;
    double reactionTimeFLR;
    double rampTimeFLR;
} Parameters;
#endif
