/*
 * NIwrapper.h
 *
 *  Created on: 15. nov. 2014
 *      Author: torstebo
 */

#ifndef NIWRAPPER_H_
#define NIWRAPPER_H_

#ifdef __cplusplus
	#define dllExport extern "C" _declspec(dllexport)
#else
  	#define dllExport 
#endif


dllExport void* thrusterAllocationInit(const unsigned short nThruster, const unsigned short nSwitchboards, const double TauMax[], const double Kt2p[], 
			const unsigned int thruster2switchboard[], const double xPosition[], const double yPosition[], const double fRampTime[], const double turnTime[], 
			const double fMin[], const double rotableD[], const double reverseThrustPowerGain[], const double sampleTime, const double singularityCostGain, const double singularityCostSmoother);

dllExport int thrusterAllocation(	void* TAobj, const double tau_d[],const double powerAvailableBus[],const double powerAvailableThruster[],const unsigned int switchboard2bus[],
			const unsigned int nbus,const double thrusterOnD[], 	const double fOld[],const double alphaOld[],double f[],double alpha[],double powerAllocatedThruster[]);

dllExport void thrusterAllocationTerminate(void* TAobj);

#endif /* NIWRAPPER_H_ */
