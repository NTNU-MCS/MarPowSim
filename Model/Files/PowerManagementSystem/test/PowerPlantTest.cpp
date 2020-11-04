/*
 * PowerPlantTest.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "Switchboard.h"
#include "PowerPlant.h"
#define uint unsigned int
namespace {
class PowerPlantTest: public ::testing::Test {
protected:
	PowerPlant p;
	unsigned int nSwitchboard;
	unsigned int nGensets;
	vector<unsigned int> s2b;
	virtual void SetUp() {
		nGensets = 2;
		nSwitchboard = 3;
		double ratedPower[] = {10., 11.};
		for (unsigned int i = 0; i < nSwitchboard; i++) {
			p.addSwitchboard(Switchboard(nGensets,ratedPower,nGensets,ratedPower,0,1));
		}
		s2b.resize(3, 0);
		s2b[0] = s2b[1] = 0;
		s2b[2] = 1;
		p.updateSwitchboard2Bus(2, s2b);
	}
};

TEST_F(PowerPlantTest, PowerPlantCheckBusSize) {
	EXPECT_EQ(p.getBus(0)->getNumberOfSwitchboard(), 2U);
	EXPECT_EQ(p.getBus(1)->getNumberOfSwitchboard(), 1U);
}

TEST_F(PowerPlantTest, PowerPlantNumberOfSwitchboard) {
	EXPECT_EQ(nSwitchboard, p.getNumberOfSwitchboards());
}

TEST_F(PowerPlantTest, PowerPlantNumberOfBus) {
	EXPECT_EQ(2, p.getNumberOfBus());
}


TEST_F(PowerPlantTest, PowerPlantUpdateLoad) {
	double powerL[] = { 1, 2, 3 };
	double powerDesiredL[] = { 4, 5, 6 };
	double powerH[] = { 7, 8, 9 };
	double powerDesiredH[] = { 10, 11, 12 };
	EXPECT_NO_THROW(
			p.updateLoads(powerH, powerDesiredH, powerL, powerDesiredL));
	for (unsigned int i = 0; i < nSwitchboard; i++) {
		EXPECT_DOUBLE_EQ(powerL[i],
				p.getSwitchboard(i)->getLoadLowPriority()->getLoad());
		EXPECT_DOUBLE_EQ(powerDesiredL[i],
				p.getSwitchboard(i)->getLoadLowPriority()->getDesiredLoad());
		EXPECT_DOUBLE_EQ(powerH[i],
				p.getSwitchboard(i)->getLoadHighPriority()->getLoad());
		EXPECT_DOUBLE_EQ(powerDesiredH[i],
				p.getSwitchboard(i)->getLoadHighPriority()->getDesiredLoad());
	}
}
TEST_F(PowerPlantTest, PowerPlantUpdateGensetsMerge) {
	double power[] = { 1, 2, 3, 4, 5, 6 };
	double powerAvailable[] = { 7, 8, 9, 10, 11, 12 };
	bool connected[] = { 1, 1, 1, 1, 1, 1 };
	EXPECT_NO_THROW(p.updateGeneratorsets(power, powerAvailable, connected));
	EXPECT_DOUBLE_EQ(power[0] + power[1] + power[2] + power[3],
	        static_cast<const Bus*>(p.getBus(0))->getGeneratorsetMerged()->getPowerGenerated());
	EXPECT_DOUBLE_EQ(power[4] + power[5],
	      static_cast<const Bus*>(p.getBus(1))->getGeneratorsetMerged()->getPowerGenerated());
	EXPECT_DOUBLE_EQ(
			powerAvailable[0] + powerAvailable[1] + powerAvailable[2]
					+ powerAvailable[3],
			static_cast<const Bus*>(p.getBus(0))->getGeneratorsetMerged()->getPowerAvailable());
	EXPECT_DOUBLE_EQ(powerAvailable[4] + powerAvailable[5],
	      static_cast<const Bus*>(p.getBus(1))->getGeneratorsetMerged()->getPowerAvailable());
}

TEST_F(PowerPlantTest, CircuitBreakerOpen){
	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 1;
	p.updateSwitchboard2Bus(2,s2b);
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 2;
	p.updateSwitchboard2Bus(3,s2b);
	EXPECT_TRUE(p.hasBusTieBreakerOpened());
	p.updateSwitchboard2Bus(3,s2b);
	EXPECT_FALSE(p.hasBusTieBreakerOpened());
}

TEST_F(PowerPlantTest, CircuitBreakerToggle){
	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 1;
	p.updateSwitchboard2Bus(2,s2b);
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 1;
	p.updateSwitchboard2Bus(2,s2b);
	EXPECT_TRUE(p.hasBusTieBreakerOpened());
	p.updateSwitchboard2Bus(2,s2b);
	EXPECT_FALSE(p.hasBusTieBreakerOpened());
}

TEST_F(PowerPlantTest, CircuitBreakerClose){
	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 1;
	p.updateSwitchboard2Bus(2,s2b);
	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 0;
	p.updateSwitchboard2Bus(1,s2b);
	EXPECT_FALSE(p.hasBusTieBreakerOpened());
	p.updateSwitchboard2Bus(1,s2b);
	EXPECT_FALSE(p.hasBusTieBreakerOpened());
}
}
