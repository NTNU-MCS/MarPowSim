/*    Copyright: 	NTNU, Trondheim, Norway
 *    Licensed under GPL-3.0
 *    Created:  	2013-2017 Torstein Aarseth Bø
 */

#ifndef NIWRAPPER_H_
#define NIWRAPPER_H_


_declspec(dllexport) void powerManagementSystem(const double nSwb,const double nGeneratorSetPerSwb[],const double ratedPowerGenset[],
    const double nThrustersPerSwb[],const double ratedPowerThrusters[],double dpPowerAvailableBias, double dpPowerAvailableGain,
    double reactionTimePA, double ramptimePA,double reactionTimeFLR, double rampTimeFLR,const double* generatorsetPowerDelivered,
    const double* generatorsetPowerAvailable,
    const double* generatorsetConnected,
    const double* powerHighPriority,
    const double* powerDesiredHighPriority,
    const double* powerLowPriority,
    const double* powerDesiredLowPriority,
    double nBus,
    const double* switchboard2Bus,
    const double* thrusterPower,
    const double time,
    const bool resetPMS,
    double* powerAvailableLowPriority,
    double* powerAvailableHighPriority,
    double* powerAvailableDP,
    double* powerConsumedDP,
    double* powerDesiredDP,
    double* powerAvailableThrusters);

#endif /* NIWRAPPER_H_ */
