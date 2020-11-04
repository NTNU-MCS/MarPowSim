/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "CircuitBreakerOpeningDetector.h"
#include <vector>
#include <float.h>

namespace {
class CircuitBreakerOpeningDetectorTest: public ::testing::Test {
protected:
	PowerPlant p;
	unsigned int nSwitchboard;
	unsigned int nGensets;
	vector<Switchboard> s;
	vector<unsigned int> s2b;
	CircuitBreakerOpeningDetector* l;

	virtual void SetUp() {
		nGensets = 2;
		nSwitchboard = 3;
		double ratedPower[] = { 1000, 1000 };
		for (unsigned int i = 0; i < nSwitchboard; i++) {
			p.addSwitchboard(
					Switchboard(nGensets, ratedPower, nGensets, ratedPower, 0.,
							1.));
		}
		s2b.resize(3, 0);
		s2b[0] = s2b[1] = 0;
		s2b[2] = 1;
		p.updateSwitchboard2Bus(2, s2b);
		double thrusterPower[6] = { 1, 2, 3, 4, 5, 6 };
		p.updateThrusters(thrusterPower);
		double powerLoad[3] = { 7, 8, 9 };
		p.updateLoads(powerLoad, powerLoad, powerLoad, powerLoad);
		l = new CircuitBreakerOpeningDetector(&p);
	}

	virtual void TearDown() {
		delete l;
	}
};
TEST_F(CircuitBreakerOpeningDetectorTest, DetectOpening) {
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 2;
	vector<bool> affectedBus(3);
	p.updateSwitchboard2Bus(3, s2b);
	EXPECT_TRUE(l->detect(affectedBus));
	EXPECT_TRUE(affectedBus[0]);
	EXPECT_TRUE(affectedBus[1]);
	EXPECT_FALSE(affectedBus[2]);

	p.updateSwitchboard2Bus(3, s2b);
	EXPECT_FALSE(l->detect(affectedBus));
}

TEST_F(CircuitBreakerOpeningDetectorTest, NotDetectClosing) {
	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 0;
	p.updateSwitchboard2Bus(1, s2b);
	vector<bool> affectedBus(1);
	EXPECT_FALSE(l->detect(affectedBus));

	p.updateSwitchboard2Bus(1, s2b);
	EXPECT_FALSE(l->detect(affectedBus));
}

TEST_F(CircuitBreakerOpeningDetectorTest, PowerAvailable2to3) {

	p.updateSwitchboard2Bus(2, s2b);
	double thrusterPower [6] = {1, 2 ,3,4,5,6};
	p.updateThrusters(thrusterPower);
	double powerLoad [3] = {7,8,9};
	p.updateLoads(powerLoad,powerLoad,powerLoad,powerLoad);

	double pd[6] = {1, 2, 3, 4, 5, 6};

	double powerBus1 = powerLoad[0]*2+powerLoad[1]*2;
	for (unsigned int i = 0; i<4;i++) {
		powerBus1 += thrusterPower[i];
	}
	pd[0]=pd[1]=pd[2]=pd[3]=powerBus1/4;

	double powerBus2 = powerLoad[3]*2+thrusterPower[4]+thrusterPower[5];
	pd[4]=pd[5]=powerBus2/2;

	double powerAvailable[] = {7, 8, 9, 10, 11, 12};
	bool connected[] = {1, 1, 1, 1, 1, 1};
	p.updateGeneratorsets(pd, powerAvailable, connected);

	double powerDelivered2[] = {13, 14, 15, 16, 17, 18};
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 2;
	p.updateSwitchboard2Bus(3, s2b);
	p.updateGeneratorsets(powerDelivered2, powerAvailable, connected);
	Fault* f = l->getFault(0);
	EXPECT_FALSE(f == 0);
	vector<double> pa = f->getPowerAvailable();

	// Power consumption is less than delivered
	powerBus1 = powerLoad[0]*2+thrusterPower[0]+thrusterPower[1];
	double deliveredPowerBus1 = pd[0]+pd[1];
	EXPECT_GT(deliveredPowerBus1,powerBus1);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), pa[0]);

	// Power consumption is greater than delivered
	powerBus2 = powerLoad[1]*2+thrusterPower[2]+thrusterPower[3];
	double deliveredPowerBus2 = pd[2]+pd[3];
	EXPECT_LT(deliveredPowerBus2,powerBus2);
	EXPECT_DOUBLE_EQ(deliveredPowerBus1, pa[1]);

	// Not effected by fault
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), pa[2]);
	delete f;
}

TEST_F(CircuitBreakerOpeningDetectorTest, FLR2to3) {

	p.updateSwitchboard2Bus(2, s2b);
	double thrusterPower [6] = {1, 2 ,3,4,5,6};
	p.updateThrusters(thrusterPower);
	double powerLoad [3] = {7,8,9};
	p.updateLoads(powerLoad,powerLoad,powerLoad,powerLoad);

	double pd[6] = {1, 2, 3, 4, 5, 6};

	double powerBus1 = powerLoad[0]*2+powerLoad[1]*2;
	for (unsigned int i = 0; i<4;i++) {
		powerBus1 += thrusterPower[i];
	}
	pd[0]=pd[1]=pd[2]=pd[3]=powerBus1/4;

	double powerBus2 = powerLoad[3]*2+thrusterPower[4]+thrusterPower[5];
	pd[4]=pd[5]=powerBus2/2;

	double powerAvailable[] = {7, 8, 9, 10, 11, 12};
	bool connected[] = {1, 1, 1, 1, 1, 1};
	p.updateGeneratorsets(pd, powerAvailable, connected);

	double powerDelivered2[] = {13, 14, 15, 16, 17, 18};
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 2;
	p.updateSwitchboard2Bus(3, s2b);
	p.updateGeneratorsets(powerDelivered2, powerAvailable, connected);
	Fault* f = l->getFault(0);
	EXPECT_FALSE(f == 0);
	vector<double> flr = f->getPowerAvailableThrusterFastLoadReduction();

	// Power consumption is less than delivered
	powerBus1 = powerLoad[0]*2+thrusterPower[0]+thrusterPower[1];
	double deliveredPowerBus1 = pd[0]+pd[1];
	EXPECT_GT(deliveredPowerBus1,powerBus1);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), flr[0]);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), flr[1]);

	// Power consumption is greater than delivered
	powerBus2 = powerLoad[1]*2+thrusterPower[2]+thrusterPower[3];
	double deliveredPowerBus2 = pd[2]+pd[3];
	EXPECT_LT(deliveredPowerBus2,powerBus2);
	EXPECT_DOUBLE_EQ(thrusterPower[2]+thrusterPower[3]-(powerBus2-deliveredPowerBus2), flr[2]+flr[3]);
	EXPECT_DOUBLE_EQ(thrusterPower[2]/flr[2], thrusterPower[3]/flr[3]);

	// Not effected by fault
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), flr[4]);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), flr[5]);
	delete f;
}

TEST_F(CircuitBreakerOpeningDetectorTest, PowerAvailable1to2) {
	p.updateSwitchboard2Bus(2, s2b);
	double thrusterPower [6] = {1, 2 ,3,4,5,6};
	p.updateThrusters(thrusterPower);
	double powerLoad [3] = {7,8,9};
	p.updateLoads(powerLoad,powerLoad,powerLoad,powerLoad);

	double pd[] = {1, 2, 3, 4, 5, 6};
	double powerAvailable[] = {7, 8, 9, 10, 11, 12};
	double powerBus1 = powerLoad[0]*2+powerLoad[1]*2+powerLoad[2]*2;;
	for (unsigned int i = 0; i<6;i++) {
		powerBus1 += thrusterPower[i];
	}
	pd[0]=pd[1]=pd[2]=pd[3]=pd[4]=pd[5]=powerBus1/6;

	for (unsigned int i = 0; i<6;i++) {
		powerAvailable[i] = pd[i]*1.1;
	}

	bool connected[] = {1, 1, 1, 1, 1, 1};
	p.updateGeneratorsets(pd, powerAvailable, connected);

	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 0;
	p.updateSwitchboard2Bus(1, s2b);
	double powerDelivered2[] = {13, 14, 15, 16, 17, 18};
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 1;
	p.updateSwitchboard2Bus(2, s2b);
	p.updateGeneratorsets(powerDelivered2, powerAvailable, connected);
	Fault* f = l->getFault(0);
	EXPECT_FALSE(f == 0);
	vector<double> res = f->getPowerAvailable();
	delete f;

	powerBus1 = powerLoad[0]*2+thrusterPower[0]+thrusterPower[1];
	double deliveredPowerBus1 = pd[0]+pd[1];
	EXPECT_GT(deliveredPowerBus1,powerBus1);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), res[0]);

	double powerBus2 = powerLoad[1]*2+powerLoad[2]*2+thrusterPower[2]+thrusterPower[3]+thrusterPower[4]+thrusterPower[5];
	double deliveredPowerBus2 = pd[2] + pd[3] + pd[4] + pd[5];
	EXPECT_LT(deliveredPowerBus2,powerBus2);
	EXPECT_DOUBLE_EQ(deliveredPowerBus2, res[1]);

}

TEST_F(CircuitBreakerOpeningDetectorTest, FLR1to2) {
	p.updateSwitchboard2Bus(2, s2b);
	double thrusterPower [6] = {1, 2 ,3,4,5,6};
	p.updateThrusters(thrusterPower);
	double powerLoad [3] = {7,8,9};
	p.updateLoads(powerLoad,powerLoad,powerLoad,powerLoad);

	double pd[] = {1, 2, 3, 4, 5, 6};
	double powerAvailable[] = {7, 8, 9, 10, 11, 12};
	double powerBus1 = powerLoad[0]*2+powerLoad[1]*2+powerLoad[2]*2;;
	for (unsigned int i = 0; i<6;i++) {
		powerBus1 += thrusterPower[i];
	}
	pd[0]=pd[1]=pd[2]=pd[3]=pd[4]=pd[5]=powerBus1/6;

	for (unsigned int i = 0; i<6;i++) {
		powerAvailable[i] = pd[i]*1.1;
	}

	bool connected[] = {1, 1, 1, 1, 1, 1};
	p.updateGeneratorsets(pd, powerAvailable, connected);

	s2b[0] = 0;
	s2b[1] = 0;
	s2b[2] = 0;
	p.updateSwitchboard2Bus(1, s2b);
	double powerDelivered2[] = {13, 14, 15, 16, 17, 18};
	s2b[0] = 0;
	s2b[1] = 1;
	s2b[2] = 1;
	p.updateSwitchboard2Bus(2, s2b);
	p.updateGeneratorsets(powerDelivered2, powerAvailable, connected);
	Fault* f = l->getFault(0);
	EXPECT_FALSE(f == 0);
	vector<double> flr = f->getPowerAvailableThrusterFastLoadReduction();
	delete f;

	powerBus1 = powerLoad[0]*2+thrusterPower[0]+thrusterPower[1];
	double deliveredPowerBus1 = pd[0]+pd[1];
	EXPECT_GT(deliveredPowerBus1,powerBus1);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), flr[0]);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(), flr[1]);

	double powerBus2 = powerLoad[1]*2+powerLoad[2]*2+thrusterPower[2]+thrusterPower[3]+thrusterPower[4]+thrusterPower[5];
	double deliveredPowerBus2 = pd[2] + pd[3] + pd[4] + pd[5];
	double thrusterPowerBus2 = thrusterPower[2]+thrusterPower[3]+thrusterPower[4]+thrusterPower[5];
	EXPECT_LT(deliveredPowerBus2,powerBus2);
	EXPECT_DOUBLE_EQ(thrusterPowerBus2-(powerBus2-deliveredPowerBus2), flr[2]+flr[3]+flr[4]+flr[5]);
	EXPECT_DOUBLE_EQ(thrusterPower[2]/flr[2], thrusterPower[3]/flr[3]);
	EXPECT_DOUBLE_EQ(thrusterPower[2]/flr[2], thrusterPower[4]/flr[4]);
	EXPECT_DOUBLE_EQ(thrusterPower[2]/flr[2], thrusterPower[5]/flr[5]);
}

}
