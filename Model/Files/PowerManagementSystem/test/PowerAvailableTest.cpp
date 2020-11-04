/*
 * PowerAvailableCalculator.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 15, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "PowerManagementSystem.h"
#include <list>
#include <vector>
#include "Fault.h"
#include "PowerAvailable.h"
#include <math.h>

using namespace std;

namespace {
class PowerAvailableTest: public ::testing::Test {
protected:
  PowerAvailable* pa;
  PowerPlant p;
  double rampTime;
  double reactionTime;
  unsigned int nSwitchboard;
  unsigned int nGensets;
  vector<Switchboard> s;
  vector<unsigned int> s2b;
  FaultList _faults;
  bool* connected1;
  double* power1;
  double* powerAvailable1;
  virtual void SetUp() {
    connected1 = new bool[6];
    connected1[0] = connected1[1] = connected1[2] = connected1[3] =
        connected1[4] = connected1[5] = 1;
    power1 = new double[6];
    for (unsigned int i = 0; i < 6; i++)
      power1[i] = i + 2;

    powerAvailable1 = new double[6];
    for (unsigned int i = 0; i < 6; i++)
      powerAvailable1[i] = i + 14;
    rampTime = 40;
    reactionTime = 1;
    nGensets = 2;
    nSwitchboard = 3;
    s.reserve(nSwitchboard);
    double ratedPower[] = { 20., 21. };
    for (unsigned int i = 0; i < nSwitchboard; i++) {
      s.push_back(Switchboard(nGensets, ratedPower, nGensets, ratedPower,0,1));
      p.addSwitchboard(s[i]);
    }
    s2b.resize(3, 0);
    s2b[0] = s2b[1] = 0;
    s2b[2] = 1;
    p.updateSwitchboard2Bus(2, s2b);
    p.updateGeneratorsets(power1, powerAvailable1, connected1);
    pa = new PowerAvailable(&_faults, &p, reactionTime, rampTime);
  }

  virtual void TearDown() {
	  delete[] connected1;
	  delete[] power1;
	  delete[] powerAvailable1;
	  delete pa;
  }
};

TEST_F(PowerAvailableTest, setPowerAvailableLoadsExact) {
  double pt[] = { 1, 2, 3 };
  double pHD[] = { 4, 5, 6 };
  double pLD[] = { 7, 8, 9 };
  double pTh[] = {10, 11, 12 ,13 ,14, 15};
  double pDPD[] = { pTh[0]+pTh[1], pTh[2]+pTh[3], pTh[4]+pTh[5] };
  p.updateThrusters(pTh);
  p.updateLoads(pt, pHD, pt, pLD);

  vector<double> PA(2);
  PA[0] = pHD[0] + pLD[0] + pDPD[0] + pHD[1] + pLD[1] + pDPD[1];
  PA[1] = pHD[2] + pLD[2] + pDPD[2];
  pa->setPowerAvailableLoads(PA);
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(pHD[i],
        p.getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(pLD[i],
        p.getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(pDPD[i],
        p.getSwitchboard(i)->getLoadDP()->getPowerAvailable());
  }
}

TEST_F(PowerAvailableTest, setPowerAvailableAllDisconnected) {
  double pt[] = { 1, 2, 3 };
  double pHD[] = { 4, 5, 6 };
  double pLD[] = { 7, 8, 9 };
  double pTh[] = {10, 11, 12 ,13 ,14, 15};
  p.updateThrusters(pTh);
  bool connected[] = {0, 0, 0, 0, 0, 0};
  p.updateGeneratorsets(power1,powerAvailable1,connected);
  p.updateLoads(pt, pHD, pt, pLD);
  pa->update(0.);
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(0,
        p.getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(0,
        p.getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(0,
        p.getSwitchboard(i)->getLoadDP()->getPowerAvailable());
  }
}

TEST_F(PowerAvailableTest, setPowerAvailableLoadsPartlyLP) {
  double pt[] = { 1, 2, 3 };
  double pHD[] = { 4, 5, 6 };
  double pLD[] = { 7, 8, 9 };
  double pTh[] = {10, 11, 12 ,13 ,14, 15};
  double pDPD[] = { pTh[0]+pTh[1], pTh[2]+pTh[3], pTh[4]+pTh[5] };
  p.updateThrusters(pTh);
  p.updateLoads(pt, pHD, pt, pLD);

  vector<double> PA(2);
  PA[0] = pHD[0] + .5 * pLD[0] + pDPD[0] + pHD[1] + .5 * pLD[1] + pDPD[1];
  PA[1] = pHD[2] + .5 * pLD[2] + pDPD[2];
  pa->setPowerAvailableLoads(PA);
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(pHD[i],
        p.getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(.5 * pLD[i],
        p.getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(pDPD[i],
        p.getSwitchboard(i)->getLoadDP()->getPowerAvailable());
  }
}

TEST_F(PowerAvailableTest, setPowerAvailableLoadsPartlyDP) {
  double pt[] = { 1, 2, 3 };
  double pHD[] = { 4, 5, 6 };
  double pLD[] = { 7, 8, 9 };
  double excessive = 1000;
  double pTh[] = {10, 11, 12 ,13 ,14, 15};
  double pDPD[] = { pTh[0]+pTh[1], pTh[2]+pTh[3], pTh[4]+pTh[5] };
  p.updateThrusters(pTh);
  p.updateLoads(pt, pHD, pt, pLD);

  vector<double> PA(2);
  PA[0] = pHD[0] + pLD[0] + pDPD[0] + pHD[1] + pLD[1] + pDPD[1] + excessive * 2;
  PA[1] = pHD[2] + pLD[2] + pDPD[2] + excessive;
  pa->setPowerAvailableLoads(PA);
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(pHD[i] + excessive / 3.,
        p.getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(pLD[i] + excessive / 3.,
        p.getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(pDPD[i] + excessive / 3.,
        p.getSwitchboard(i)->getLoadDP()->getPowerAvailable());
  }
}

TEST_F(PowerAvailableTest, setPowerAvailableLoadsPartlyHP) {
  double pt[] = { 1, 2, 3 };
  double pHD[] = { 4, 5, 6 };
  double pLD[] = { 7, 8, 9 };
  p.updateLoads(pt, pHD, pt, pLD);
  double pTh[] = {10, 11, 12 ,13 ,14, 15};
  double pDPD[] = { pTh[0]+pTh[1], pTh[2]+pTh[3], pTh[4]+pTh[5] };
  p.updateThrusters(pTh);
  vector<double> PA(2);
  PA[0] = .5 * pHD[0] + 0 * pLD[0] + 0 * pDPD[0] + .5 * pHD[1] + 0 * pLD[1]
      + 0 * pDPD[1];
  PA[1] = .5 * pHD[2] + 0 * pLD[2] + 0 * pDPD[2];
  pa->setPowerAvailableLoads(PA);
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(.5 * pHD[i],
        p.getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(0,
        p.getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(0, p.getSwitchboard(i)->getLoadDP()->getPowerAvailable());
  }
}

TEST_F(PowerAvailableTest, setPowerAvailableLoadsExcessive) {
  double pt[] = { 1, 2, 3 };
  double pHD[] = { 4, 5, 6 };
  double pLD[] = { 7, 8, 9 };
  p.updateLoads(pt, pHD, pt, pLD);
  double pTh[] = {10, 11, 12 ,13 ,14, 15};
  double pDPD[] = { pTh[0]+pTh[1], pTh[2]+pTh[3], pTh[4]+pTh[5] };
  p.updateThrusters(pTh);
  vector<double> PA(2);
  PA[0] = .5 * pHD[0] + 0 * pLD[0] + 0 * pDPD[0] + .5 * pHD[1] + 0 * pLD[1]
      + 0 * pDPD[1];
  PA[1] = .5 * pHD[2] + 0 * pLD[2] + 0 * pDPD[2];
  pa->setPowerAvailableLoads(PA);
  for (int i = 0; i < 3; i++) {
    EXPECT_DOUBLE_EQ(.5 * pHD[i],
        p.getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(0,
        p.getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable());
    EXPECT_DOUBLE_EQ(0, p.getSwitchboard(i)->getLoadDP()->getPowerAvailable());
  }
}



TEST_F(PowerAvailableTest, powerAvailableBus_NoFault) {
  vector<double> res = pa->calculatePowerAvailableBus(0.);
  ASSERT_DOUBLE_EQ(
      powerAvailable1[0] + powerAvailable1[1] + powerAvailable1[2]
          + powerAvailable1[3], res[0]);
  ASSERT_DOUBLE_EQ(powerAvailable1[4] + powerAvailable1[5], res[1]);
}

TEST_F(PowerAvailableTest, powerAvailableBus_FaultAtReactionTime) {
  vector<double> powerA(2);
  powerA[0] = 1.;
  powerA[1] = 2.;
  vector<double> tmp(0);
  Fault* f = new Fault(0., powerA, tmp, tmp);
  _faults.push_back(f);
  vector<double> res = pa->calculatePowerAvailableBus(reactionTime);
  ASSERT_DOUBLE_EQ(powerA[0], res[0]);
  ASSERT_DOUBLE_EQ(powerA[1], res[1]);
}

TEST_F(PowerAvailableTest, powerAvailableBus_FaultAtFaultTime) {
  vector<double> powerA(2);
  powerA[0] = 1.;
  powerA[1] = 2.;
  vector<double> tmp(0);
  Fault* f = new Fault(0., powerA, tmp, tmp);
  _faults.push_back(f);
  vector<double> res = pa->calculatePowerAvailableBus(0.);
  ASSERT_DOUBLE_EQ(
      powerAvailable1[0] + powerAvailable1[1] + powerAvailable1[2]
          + powerAvailable1[3], res[0]);
  ASSERT_DOUBLE_EQ(powerAvailable1[4] + powerAvailable1[5], res[1]);
}

TEST_F(PowerAvailableTest, powerAvailableBus_FaultBeforeReactionTime) {
  vector<double> powerA(2);
  powerA[0] = 1.;
  powerA[1] = 2.;
  vector<double> tmp(0);
  Fault* f = new Fault(0., powerA, tmp, tmp);
  _faults.push_back(f);
  vector<double> res = pa->calculatePowerAvailableBus(reactionTime*.99);
  ASSERT_DOUBLE_EQ(
      powerAvailable1[0] + powerAvailable1[1] + powerAvailable1[2]
          + powerAvailable1[3], res[0]);
  ASSERT_DOUBLE_EQ(powerAvailable1[4] + powerAvailable1[5], res[1]);
}

TEST_F(PowerAvailableTest, powerAvailableBus_FaultAfterRampTime) {
  vector<double> powerA(2);
  powerA[0] = 1.;
  powerA[1] = 2.;
  vector<double> tmp(0);
  Fault* f = new Fault(0., powerA, tmp, tmp);
  _faults.push_back(f);
  vector<double> res = pa->calculatePowerAvailableBus(reactionTime+rampTime);
  ASSERT_DOUBLE_EQ(
      powerAvailable1[0] + powerAvailable1[1] + powerAvailable1[2]
          + powerAvailable1[3], res[0]);
  ASSERT_DOUBLE_EQ(powerAvailable1[4] + powerAvailable1[5], res[1]);
}

TEST_F(PowerAvailableTest, powerAvailableBus_FaultAtReactionTimeHigh) {
  vector<double> powerA(2);
  powerA[0] = powerA[1] = 1e6;
  vector<double> tmp(0.);
  Fault* f = new Fault(0., powerA, tmp, tmp);
  _faults.push_back(f);
  vector<double> res = pa->calculatePowerAvailableBus(reactionTime);
  ASSERT_DOUBLE_EQ(
      powerAvailable1[0] + powerAvailable1[1] + powerAvailable1[2]
          + powerAvailable1[3], res[0]);
  ASSERT_DOUBLE_EQ(powerAvailable1[4] + powerAvailable1[5], res[1]);
}

TEST_F(PowerAvailableTest, powerAvailableBus_FaultAtReactionTimeHighAndLow) {
  vector<double> powerA(2);
  powerA[0] = 1e6;
  powerA[1] = 1;
  vector<double> tmp(0.);
  Fault* f = new Fault(0., powerA, tmp, tmp);
  _faults.push_back(f);
  vector<double> res = pa->calculatePowerAvailableBus(reactionTime);
  ASSERT_DOUBLE_EQ(
      powerAvailable1[0] + powerAvailable1[1] + powerAvailable1[2]
          + powerAvailable1[3], res[0]);
  ASSERT_DOUBLE_EQ(powerA[1], res[1]);
}
}

