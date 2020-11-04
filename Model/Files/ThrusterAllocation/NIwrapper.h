/*
 * NIwrapper.h
 *
 *  Created on: 15. nov. 2014
 *      Author: torstebo
 */

#ifndef NIWRAPPER_H_
#define NIWRAPPER_H_


_declspec(dllexport) bool thrusterAllocation(bool resetTA,const unsigned short nThruster, ///< Number of thrusters on vessel
			const unsigned short nSwitchboards,       ///< Number of switchboards
			const double TauMax[],      ///< Maximum thrust of each thruster [m]
			const double Kt2p[], ///< Thrust to power gain [W/N^1.5] power is assumed to be P = Kt2p |T|^1.5
			const unsigned int thruster2switchboard[], ///< connectivity vector, index is index of thruster, value is index of switchboard
			const double xPosition[], ///< x-Position of thruster[m]
			const double yPosition[], ///< y-Position of thruster [m]
			const double fRampTime[], ///< Minimum time to go from 0 thrust to maximum thrust [s]
			const double turnTime[], ///< Minimum time to turn thruster 360 degrees [s]
			const double fMin[], ///< Minimum thrust demand (maximum is set to 1)
			const short rotable[], ///< True if thruster is rotable
			const double reverseThrustPowerGain[], ///< Extra power used when thrust is reversed
			const double sampleTime, ///< Sample time of thruster allocation
			const double singularityCostGain, ///< Singularity cost gain
			const double singularityCostSmoother, ///< Singularity cost smoother (1/(smoother+|x|))
			const double tau_d[], ///< Desired thrust in surge, sway and. yaw [N,N,Nm]
			const double powerAvailableBus[], ///< Power reserved for thrust at each bus [W]
			const double powerAvailableThruster[], ///< Power reserved for each thruster [W]
			const unsigned int switchboard2bus[], ///< Mapping from switchboard index to bus index, index is switchboard index, value is bus index
			const unsigned int nbus, ///< Number of busses
			const short thrusterOn[], ///< Vector of thrusters on, index is thruster index, value=1 if thruster is on 0 if thurster is off
			const double fOld[], ///< Current thrust command
			const double alphaOld[], ///< Current azimuth angles [rad]
			double f[], ///< [out] Thruster demand [-1..1]
			double alpha[], ///< [out]  Azimuth angle of thruster [rad]
			double powerAllocatedThruster[] ///[out] Maximum power used by each thruster [W]
			);

#endif /* NIWRAPPER_H_ */
