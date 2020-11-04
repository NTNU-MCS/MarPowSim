
#include <windows.h>
#include <vector>
#include "src/PowerPlant.h"
#include "src/Switchboard.h"
#include "src/PowerManagementSystem.h"
#define MAX_SWB 100
#define __cplusplus

#ifdef __cplusplus
extern "C" {
#endif
#include "cwrapper.h"
void* pmsInitWrapper(const double nSwbD,const double nGeneratorSetPerSwbD[],const double ratedPowerGenset[],
    const double nThrustersPerSwbD[],const double ratedPowerThrusters[],double dpPowerAvailableBias, double dpPowerAvailableGain,
    double reactionTimePA, double ramptimePA,double reactionTimeFLR, double rampTimeFLR)
{

  const unsigned int nSwb = nSwbD;
  //ASSERT(nSwb<=MAX_SWB);
  unsigned int nGeneratorSetPerSwb[MAX_SWB];
  unsigned int nThrustersPerSwb[MAX_SWB];
  for (unsigned int i = 0; i<nSwb;i++){
    nGeneratorSetPerSwb[i] = nGeneratorSetPerSwbD[i];
    nThrustersPerSwb[i] = nThrustersPerSwbD[i];
  }
  PowerManagementSystem* pPms = 0;
  unsigned int nThrusters = 0;
    PowerPlant p;
    unsigned int nGensetAdded = 0;
    for (unsigned int i = 0; i<nSwb; i++) {
      Switchboard s(nGeneratorSetPerSwb[i],ratedPowerGenset,nThrustersPerSwb[i],ratedPowerThrusters,dpPowerAvailableBias,dpPowerAvailableGain);
      ratedPowerGenset += nGeneratorSetPerSwb[i];
      ratedPowerThrusters += nThrustersPerSwb[i];
      nThrusters += nThrustersPerSwb[i];
      p.addSwitchboard(s);
    }

  return (void*) new PowerManagementSystem(p,reactionTimePA, ramptimePA, reactionTimeFLR, rampTimeFLR);
}

void pmsDeleteWrapper(void* obj){
  if (obj != 0x0){
  PowerManagementSystem* pPms = (PowerManagementSystem*) obj;
  delete pPms;
  }
}

_declspec(dllexport) void pmsWrapper(void* obj, const double* generatorsetPowerDelivered,
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
    double* powerAvailableThrusters)
{

  const unsigned int nBus = nBusD;
  PowerManagementSystem* pPms = (PowerManagementSystem*) obj;

  const PowerPlant* p = pPms->getPowerPlant();
  const unsigned int nThrusters = p->getNumberOfThrusters();
  const unsigned int nSwb = p->getNumberOfSwitchboards();

  bool generatorsOn[4000];
  for (unsigned int i = 0; i < nThrusters;i++)
    generatorsOn[i] = generatorsetConnected[i] > 0;

  vector<unsigned int> switchboard2BusV(nSwb);
  for (unsigned int i = 0; i < nSwb; i++)
    switchboard2BusV[i] = switchboard2BusD[i];


  pPms->update(generatorsetPowerDelivered,
      generatorsetPowerAvailable,
      generatorsOn,
      powerHighPriority,
      powerDesiredHighPriority,
      powerLowPriority,
      powerDesiredLowPriority,
      nBus,
      switchboard2BusV,
      thrusterPower,
      time);

  for (unsigned int i = 0; i < nSwb; i++) {
    powerAvailableLowPriority[i] =
    p->getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable();
    powerAvailableHighPriority[i] =
    p->getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable();
    if(i<nBus) {
      const LoadMerger<DPLoad>* l = p->getBus(i)->getLoadDP();
      powerAvailableDP[i] = l->getPowerAvailable();
      powerConsumedDP[i] = l->getLoad();
      powerDesiredDP[i] = l->getDesiredLoad();
    }
    else {
      powerAvailableDP[i] = 0;
      powerConsumedDP[i] = 0;
      powerDesiredDP[i] = 0;
    }
  }
  for (unsigned int i = 0; i < nThrusters; i++) {
    powerAvailableThrusters[i] = p->getThruster(i)->getPowerAvailable();
    }

}

#ifdef __cplusplus
}
#endif
