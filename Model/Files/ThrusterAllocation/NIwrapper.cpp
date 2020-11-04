#include "NIwrapper.h"
#include <windows.h>
#include <vector>
#include "src/ThrustAllocation.h"

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
			const short rotableI[], ///< True if thruster is rotable
			const double reverseThrustPowerGain[], ///< Extra power used when thrust is reversed
			const double sampleTime, ///< Sample time of thruster allocation
			const double singularityCostGain, ///< Singularity cost gain
			const double singularityCostSmoother, ///< Singularity cost smoother (1/(smoother+|x|))
			const double tau_d[], ///< Desired thrust in surge, sway and. yaw [N,N,Nm]
			const double powerAvailableBus[], ///< Power reserved for thrust at each bus [W]
			const double powerAvailableThruster[], ///< Power reserved for each thruster [W]
			const unsigned int switchboard2bus[], ///< Mapping from switchboard index to bus index, index is switchboard index, value is bus index
			const unsigned int nbus, ///< Number of busses
			const short thrusterOnS[], ///< Vector of thrusters on, index is thruster index, value=1 if thruster is on 0 if thurster is off
			const double fOld[], ///< Current thrust command
			const double alphaOld[], ///< Current azimuth angles [rad]
			double f[], ///< [out] Thruster demand [-1..1]
			double alpha[], ///< [out]  Azimuth angle of thruster [rad]
			double powerAllocatedThruster[] ///[out] Maximum power used by each thruster [W]
			)
{
  static ThrustAllocation* pTA = 0;
  if(pTA == 0 || resetTA) {
    if (resetTA && pTA != 0 ){
      delete pTA;
	  pTA = 0x0;
    }
	bool rotable[4000];
	for (unsigned int i = 0; i<nThruster;i++)
		rotable[i] = rotableI[i]>0;

	pTA = new ThrustAllocation(nThruster, ///< Number of thrusters on vessel
			nSwitchboards,       ///< Number of switchboards
			TauMax,      ///< Maximum thrust of each thruster [N]
			Kt2p, ///< Thrust to power gain [W/N^1.5] power is assumed to be P = Kt2p |T|^1.5
			thruster2switchboard, ///< connectivity vector, index is index of thruster, value is index of switchboard
			xPosition, ///< x-Position of thruster[m]
			yPosition, ///< y-Position of thruster [m]
			fRampTime, ///< Minimum time to go from 0 thrust to maximum thrust [s]
			turnTime, ///< Minimum time to turn thruster 360 degrees [s]
			fMin, ///< Minimum thrust demand (maximum is set to 1)
			rotable, ///< True if thruster is rotable
			reverseThrustPowerGain, ///< Extra power used when thrust is reversed
			sampleTime, ///< Sample time of thruster allocation
			singularityCostGain, ///< Singularity cost gain
			singularityCostSmoother ///< Singularity cost smoother (1/(smoother+|x|))
			);
	pTA->initialize(fOld, alphaOld);
  }
  bool thrusterOn[4000];
  for (unsigned int i = 0; i<nThruster;i++)
		thrusterOn[i] = thrusterOnS[i]>0;
  return pTA->thrust(tau_d,powerAvailableBus,powerAvailableThruster,switchboard2bus,nbus,thrusterOn,fOld,alphaOld,f,alpha,powerAllocatedThruster);

}


