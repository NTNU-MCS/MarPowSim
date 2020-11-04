/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "LossOfGeneratorsetDetector.h"
namespace {
class LossOfGeneratorsetDetectorTest: public ::testing::Test {
protected:
	PowerPlant p;
	unsigned int nSwitchboard;
	unsigned int nGensets;
	unsigned int nThrusters;
	vector<unsigned int> s2b;
	LossOfGeneratorSetDetector* l;
	vector<double> thrusterRatedPower;

	virtual void SetUp() {
		nGensets = 2;
		nThrusters = 2;
		nSwitchboard = 3;
		thrusterRatedPower.push_back(12.);
		thrusterRatedPower.push_back(13.);
		thrusterRatedPower.push_back(14.);
		thrusterRatedPower.push_back(15.);
		thrusterRatedPower.push_back(16.);
		thrusterRatedPower.push_back(17.);
		double ratedPower[] = { 10., 11. };
		for (unsigned int i = 0; i < nSwitchboard; i++) {
			p.addSwitchboard(
					Switchboard(nGensets, ratedPower, nThrusters,
							&thrusterRatedPower[i * 2], 0., 1.));
		}
		s2b.resize(3, 0);
		s2b[0] = s2b[1] = 0;
		s2b[2] = 1;
		p.updateSwitchboard2Bus(2, s2b);
		l = new LossOfGeneratorSetDetector(&p);
	}

	virtual void TearDown() {
		delete l;
	}
};
TEST_F(LossOfGeneratorsetDetectorTest, Loss) {
	bool connected[] = {1, 1, 1, 1, 1, 1};
	double powerDummy[] = {2,3,4,5,6,7};
	p.updateGeneratorsets(powerDummy,powerDummy,connected);
	connected[3] = false;
	p.updateGeneratorsets(powerDummy,powerDummy,connected);
	vector<bool> affectedBus(2);
	EXPECT_TRUE(l->detect(affectedBus));
	EXPECT_TRUE(affectedBus[0]);
	EXPECT_FALSE(affectedBus[1]);
}

TEST_F(LossOfGeneratorsetDetectorTest, NoLoss) {
	bool connected[] = {1, 1, 1, 1, 1, 1};
	double powerDummy[] = {2,3,4,5,6,7};
	p.updateGeneratorsets(powerDummy,powerDummy,connected);
	p.updateGeneratorsets(powerDummy,powerDummy,connected);
	vector<bool> affectedBus(2);
	EXPECT_FALSE(l->detect(affectedBus));
}

TEST_F(LossOfGeneratorsetDetectorTest, Adding) {
	bool connected[] = {0, 1, 1, 1, 1, 1};
	double powerDummy[] = {2,3,4,5,6,7};
	p.updateGeneratorsets(powerDummy,powerDummy,connected);
	connected[0] = true;
	p.updateGeneratorsets(powerDummy,powerDummy,connected);
	vector<bool> affectedBus(2);
	EXPECT_FALSE(l->detect(affectedBus));
}

TEST_F(LossOfGeneratorsetDetectorTest, FLR) {
	bool connected[] = {1, 1, 1, 1, 1, 1};
	double po[] = {20,21,22,23,24,25};
	double powA [] = {14,15,16,17,18,19};
	double powTh [] = {2,3,4,5,6,7};
	double powLP [] = {1, 2, 3};
	double powHP [] = {4, 5, 6};
	double tmp = (powTh[0]+powTh[1]+powTh[2]+powTh[3]+powLP[0]+powLP[1]+powHP[0]+powHP[1])/4;
	po[0] = po[1] = po[2] = po[3] = tmp;
	tmp = (powTh[4]+powTh[5]+powLP[2]+powHP[2])/2;
	po[4] = po[5] = tmp;
	p.updateThrusters(powTh);
	p.updateLoads(powHP,powHP,powLP,powLP);

	p.updateGeneratorsets(po,powA,connected);
	connected[3] = false;
	double powerDummy2[] = {8,9,10,11,12,13};
	p.updateGeneratorsets(powerDummy2,powA,connected);

	Fault* f = l->getFault(0.);
	EXPECT_FALSE(l == 0);
	vector<double> flr = f->getPowerAvailableThrusterFastLoadReduction();
	double sumFLR = flr[0]+flr[1]+flr[2]+flr[3];
	double sumThr = powTh[0]+powTh[1]+powTh[2]+powTh[3];
	EXPECT_DOUBLE_EQ(sumThr-po[3],sumFLR);
	EXPECT_DOUBLE_EQ(flr[0]/powTh[0],flr[1]/powTh[1]);
	EXPECT_DOUBLE_EQ(flr[0]/powTh[0],flr[2]/powTh[2]);
	EXPECT_DOUBLE_EQ(flr[0]/powTh[0],flr[3]/powTh[3]);

	// On another bus
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(),flr[4]);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(),flr[5]);
	delete f;
}

TEST_F(LossOfGeneratorsetDetectorTest, FaultLoss) {
	bool connected[] = {1, 1, 1, 1, 1, 1};
	double po[] = {2,3,4,5,6,7};
	double powA [] = {14,15,16,17,18,19};
	double poLoad [] = {1000.,1000.,1000.};
	double poThr [] = {1001,1002,1003,1004,1005,1006};
	p.updateLoads(poLoad,poLoad,poLoad,poLoad);
	p.updateThrusters(poThr);

	p.updateGeneratorsets(po,powA,connected);
	connected[3] = false;
	double powerDummy2[] = {8,9,10,11,12,13};
	p.updateGeneratorsets(powerDummy2,powA,connected);
	double t = 156.90;

	Fault* f = l->getFault(t);
	EXPECT_TRUE(f!=0);
	EXPECT_EQ(t,f->getTime());
	vector<double> powerAvailable = f->getPowerAvailable();
	EXPECT_DOUBLE_EQ(po[0]+po[1]+po[2],powerAvailable[0]);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(),powerAvailable[1]);
	delete f;
}

TEST_F(LossOfGeneratorsetDetectorTest, FaultDeadBus) {
	bool connected[] = {1, 1, 1, 1, 1, 1};
	double po[] = {2,3,4,5,6,7};
	double powA [] = {14,15,16,17,18,19};
	double poLoad [] = {1000.,1000.,1000.};
	double poThr [] = {1001,1002,1003,1004,1005,1006};
	p.updateLoads(poLoad,poLoad,poLoad,poLoad);
	p.updateThrusters(poThr);
	p.updateGeneratorsets(po,powA,connected);
	connected[0] = connected[1] = connected[2] = connected[3] =false;
	double powerDummy2[] = {8,9,10,11,12,13};
	p.updateGeneratorsets(powerDummy2,powA,connected);
	double t = 156.90;
	Fault* f = l->getFault(t);
	EXPECT_TRUE(f!=0);
	EXPECT_EQ(t,f->getTime());
	vector<double> powerAvailable = f->getPowerAvailable();
	EXPECT_DOUBLE_EQ(0.,powerAvailable[0]);
	EXPECT_DOUBLE_EQ(numeric_limits<double>::infinity(),powerAvailable[1]);
	delete f;
}

TEST_F(LossOfGeneratorsetDetectorTest, FaultNOLoss) {
	bool connected[] = {1, 1, 0, 1, 1, 1};
	double po[] = {2,3,4,5,6,7};
	double powA [] = {14,15,16,17,18,19};
	p.updateGeneratorsets(po,powA,connected);
	double powerDummy2[] = {8,9,10,11,12,13};
	p.updateGeneratorsets(powerDummy2,powA,connected);
	EXPECT_TRUE(l->getFault(0)==0);
}

}
