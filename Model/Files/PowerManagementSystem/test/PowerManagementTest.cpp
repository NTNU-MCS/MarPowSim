/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "PowerManagementSystem.h"
#include <vector>
#include "Fault.h"

namespace {
  class PowerManagementSystemTest : public ::testing::Test {
  protected:
    PowerManagementSystem* pms;

	unsigned int nSwitchboard;
	unsigned int nGensets;
	unsigned int nThrusters;
	unsigned int nBatteries;
	vector<unsigned int> s2b;
	virtual void SetUp() {
		PowerPlant p;
		nGensets = 3;
		nSwitchboard = 3;
		nThrusters = 2.;
		nBatteries = 1.;
		double ratedPowerG[] = {10., 11.,12.};
		double ratedPowerT[] = {13., 14.};
		for (unsigned int i = 0; i < nSwitchboard; i++) {
			p.addSwitchboard(Switchboard(nGensets,ratedPowerG,nThrusters,ratedPowerT,nBatteries,0,1));
		}
		s2b.resize(3, 0);
		s2b[0] = s2b[1] = 0;
		s2b[2] = 1;
		p.updateSwitchboard2Bus(2, s2b);
		pms = new PowerManagementSystem(p,1,45.,5.,.5,true);
	}

	virtual void TearDown() {
		delete pms;
	}
  };
  TEST_F(PowerManagementSystemTest, NoThrow) {
 	  	bool connected[] = {1, 1, 1, 1, 1, 1,1,1,1};
 	  	double powG[9] = {2,3,4,5,6,7,8,9,10};
 	  	double powAG [9] = {14,15,16,17,18,19,20,21,23};
 	  	double powT[6] = {21,22,23,24,25,26};
 	  	double powB[3] = {31,32,33};
 	  	double powBA[3] = {34,35,36};
 	  	double powHP[3] = {41,42,43};
 	  	double powHPD[3] = {51,52,53};
 		double t = 156.90;
 		EXPECT_NO_FATAL_FAILURE(pms->update(powG,powAG,connected,powB,powBA,powHP,powHPD,powHP,powHPD,3,s2b,powT,t));
   }


}
