/*    Copyright: 	NTNU, Trondheim, Norway
 *    Licensed under GPL-3.0
 *    Created:  	2013-2017 Torstein Aarseth Bø
 */

#include "NIwrapper.h"
#include <windows.h>
#include <vector>
#include "src/PowerPlant.h"
#include "src/Switchboard.h"
#include "src/PowerManagementSystem.h"
#define MAX_SWB 100

_declspec(dllexport) void powerManagementSystem(const double nSwbD,const double nGeneratorSetPerSwbD[],const double ratedPowerGenset[],
    const double nThrustersPerSwbD[],const double ratedPowerThrusters[],double dpPowerAvailableBias, double dpPowerAvailableGain,
    double reactionTimePA, double ramptimePA,double reactionTimeFLR, double rampTimeFLR,const double* generatorsetPowerDelivered,
    const double* generatorsetPowerAvailable,
    const double* generatorsetConnected,
    const double* powerHighPriority,
    const double* powerDesiredHighPriority,
    const double* powerLowPriority,
    const double* powerDesiredLowPriority,
    const double nBusD,
    const double switchboard2BusD[],
    const double* thrusterPower,
    const double &time,
    const bool resetPMS,
    double* powerAvailableLowPriority,
    double* powerAvailableHighPriority,
    double* powerAvailableDP,
    double* powerConsumedDP,
    double* powerDesiredDP,
    double* powerAvailableThrusters)
{

  const unsigned int nSwb = nSwbD;
  const unsigned int nBus = nBusD;
  //ASSERT(nSwb<=MAX_SWB);
  unsigned int nGeneratorSetPerSwb[MAX_SWB];
  unsigned int nThrustersPerSwb[MAX_SWB];
  unsigned int switchboard2Bus[MAX_SWB];
  for (unsigned int i = 0; i<nSwb;i++){
    nGeneratorSetPerSwb[i] = nGeneratorSetPerSwbD[i];
    nThrustersPerSwb[i] = nThrustersPerSwbD[i];
    switchboard2Bus[i] = switchboard2BusD[i];
  }
  static PowerManagementSystem* pPms = 0;
  static unsigned int nThrusters = 0;
  if(pPms == 0 || resetPMS) {
    if (resetPMS && pPms != 0 ){
      delete pPms;
      nThrusters = 0;
    }
    PowerPlant p;
    unsigned int nGensetAdded = 0;
    for (unsigned int i = 0; i<nSwb; i++) {
      Switchboard s(nGeneratorSetPerSwb[i],ratedPowerGenset,nThrustersPerSwb[i],ratedPowerThrusters,dpPowerAvailableBias,dpPowerAvailableGain);
      ratedPowerGenset += nGeneratorSetPerSwb[i];
      ratedPowerThrusters += nThrustersPerSwb[i];
      nThrusters += nThrustersPerSwb[i];
      p.addSwitchboard(s);
    }

    pPms = new PowerManagementSystem(p,reactionTimePA, ramptimePA, reactionTimeFLR, rampTimeFLR,true);
  }
  bool generatorsOn[4000];
  for (unsigned int i = 0; i < nThrusters;i++)
  generatorsOn[i] = generatorsetConnected[i] > 0;

  vector<unsigned int> switchboard2BusV(nSwb);
  for (unsigned int i = 0; i < nSwb; i++)
  switchboard2BusV[i] = switchboard2Bus[i];
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
  const PowerPlant* p = pPms->getPowerPlant();
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


