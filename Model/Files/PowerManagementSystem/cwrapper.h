/*    Copyright: 	NTNU, Trondheim, Norway
 *    Licensed under GPL-3.0
 *    Created:  	2013-2017 Torstein Aarseth Bø
 */

#ifndef NIWRAPPER_H_
#define NIWRAPPER_H_


extern "C" _declspec(dllexport) void* __cdecl pmsInitWrapper(const double nSwbD,const double nGeneratorSetPerSwbD[],const double ratedPowerGenset[],
    const double nThrustersPerSwbD[],const double ratedPowerThrusters[],double dpPowerAvailableBias, double dpPowerAvailableGain,
    double reactionTimePA, double ramptimePA,double reactionTimeFLR, double rampTimeFLR);


extern "C" _declspec(dllexport) void pmsWrapper(void* obj, const double* generatorsetPowerDelivered,
    const double* generatorsetPowerAvailable,
    const double* generatorsetConnected,
    const double* powerHighPriority,
    const double* powerDesiredHighPriority,
    const double* powerLowPriority,
    const double* powerDesiredLowPriority,
    const double nBusD,
    const double switchboard2BusD[],
    const double* thrusterPower,
    const double time,
    double* powerAvailableLowPriority,
    double* powerAvailableHighPriority,
    double* powerAvailableDP,
    double* powerConsumedDP,
    double* powerDesiredDP,
    double* powerAvailableThrusters);

extern "C" _declspec(dllexport) void __cdecl pmsDeleteWrapper(void* obj);

#endif /* NIWRAPPER_H_ */
