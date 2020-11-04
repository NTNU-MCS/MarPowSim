#include "NIwrapper.h"
#include <windows.h>
#include <vector>
#include "src/ThrustAllocation.h"
#define MAX_N_THRUSTERS 100

extern "C" {
#include "cwrapper.h"
_declspec(dllexport) void* thrusterAllocationInit(const unsigned int nThruster, ///< Number of thrusters on vessel
			const unsigned int nSwitchboards,       ///< Number of switchboards
			const double TauMax[],      ///< Maximum thrust of each thruster [m]
			const double Kt2p[], ///< Thrust to power gain [W/N^1.5] power is assumed to be P = Kt2p |T|^1.5
			const unsigned int thruster2switchboard[], ///< connectivity vector, index is index of thruster, value is index of switchboard
			const double xPosition[], ///< x-Position of thruster[m]
			const double yPosition[], ///< y-Position of thruster [m]
			const double fRampTime[], ///< Minimum time to go from 0 thrust to maximum thrust [s]
			const double turnTime[], ///< Minimum time to turn thruster 360 degrees [s]
			const double fMin[], ///< Minimum thrust demand (maximum is set to 1)
			const double rotableD[], ///< True if thruster is rotable
			const double reverseThrustPowerGain[], ///< Extra power used when thrust is reversed
			const double sampleTime, ///< Sample time of thruster allocation
			const double singularityCostGain, ///< Singularity cost gain
			const double singularityCostSmoother ///< Singularity cost smoother (1/(smoother+|x|))
			)
{
	bool rotable[MAX_N_THRUSTERS];
	for (unsigned int i = 0; i < MAX_N_THRUSTERS; i++){
		rotable[i] = rotableD[i]>.5;
	}

	return (void*) new ThrustAllocation(nThruster, ///< Number of thrusters on vessel
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

}

_declspec(dllexport) int thrusterAllocation(
			void* TAobj,
			const double tau_d[], ///< Desired thrust in surge, sway and. yaw [N,N,Nm]
			const double powerAvailableBus[], ///< Power reserved for thrust at each bus [W]
			const double powerAvailableThruster[], ///< Power reserved for each thruster [W]
			const unsigned int switchboard2bus[], ///< Mapping from switchboard index to bus index, index is switchboard index, value is bus index
			const unsigned int nbus, ///< Number of busses
			const double thrusterOnD[], ///< Vector of thrusters on, index is thruster index, value=1 if thruster is on 0 if thurster is off
			const double fOld[], ///< Current thrust command
			const double alphaOld[], ///< Current azimuth angles [rad]
			double f[], ///< [out] Thruster demand [-1..1]
			double alpha[], ///< [out]  Azimuth angle of thruster [rad]
			double powerAllocatedThruster[] ///[out] Maximum power used by each thruster [W]
			){
			bool thrusterOn[MAX_N_THRUSTERS];
			for (unsigned int i = 0; i < MAX_N_THRUSTERS; i++){
				thrusterOn[i] = thrusterOnD[i] > 0.5;
			}
			ThrustAllocation* pTA = (ThrustAllocation*) TAobj;
			static bool firstRun = 1;
			if (firstRun){
				pTA->initialize(fOld,alphaOld);
				firstRun = 0;
			}
			return pTA->thrust(tau_d,powerAvailableBus,powerAvailableThruster,switchboard2bus,nbus,thrusterOn,fOld,alphaOld,f,alpha,powerAllocatedThruster);
}




_declspec(dllexport) void thrusterAllocationTerminate(void* TAobj) {
	if (TAobj != 0){
		ThrustAllocation* pTA = (ThrustAllocation*) TAobj;
		delete pTA;
		TAobj = 0;
	}
}
}