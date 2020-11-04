/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "Switchboard.h"
#include "FastLoadReduction.h"

namespace {
class FastLoadReductionTest: public ::testing::Test {
protected:
	FastLoadReduction* flr;
	PowerPlant* p;
	double rampTime;
	double restoreTime;
	unsigned int nSwitchboard;
	unsigned int nThrusters;
	vector<unsigned int> s2b;
	FaultList _faults;
	vector<bool> connected1;
	vector<double> power1;
	vector<double> powerAvailable1;
	vector<double> ratedPower;
	virtual void SetUp() {
		connected1 = vector<bool>(6);
		connected1[0] = connected1[1] = connected1[2] = connected1[3] =
				connected1[4] = connected1[5] = 1;
		power1 = vector<double>(6);
		for (unsigned int i = 0; i < 6; i++)
			power1[i] = i + 2;

		powerAvailable1 = vector<double>(6);
		for (unsigned int i = 0; i < 6; i++)
			powerAvailable1[i] = i + 14;
		rampTime = 10;
		rampTime = 10;
		restoreTime = 15;

		nThrusters = 2;
		nSwitchboard = 3;
		ratedPower = vector<double>(2);
		ratedPower[0] = 20.;
		ratedPower[1] = 21.;

		p = new PowerPlant();
		for (unsigned int i = 0; i < nSwitchboard; i++) {
			double tmp = 1.;
			p->addSwitchboard(Switchboard(1, &tmp, nThrusters, &ratedPower[0],0.,1.));
		}
		s2b.resize(3, 0);
		s2b[0] = s2b[1] = 0;
		s2b[2] = 1;
		p->updateSwitchboard2Bus(2, s2b);
		p->updateThrusters(&power1[0]);
		flr = new FastLoadReduction(restoreTime, rampTime, p, &_faults);
		vector<double> tmp(1, 0.);
		_faults.push_back(new Fault(0., tmp, power1, powerAvailable1));

	}

	virtual void TearDown() {
		delete flr;
		delete p;
	}
};

TEST_F(FastLoadReductionTest, flr_atFault) {
	double res[6];
	flr->calculatePowerAvailable(0., res);
	for (unsigned int i = 0; i < 6; i++)
		ASSERT_DOUBLE_EQ(ratedPower[i % 2], res[i]);
}

TEST_F(FastLoadReductionTest, flr_notReactedFault) {
	double res[6];
	flr->calculatePowerAvailable(0.+restoreTime*.5, res);
	for (unsigned int i = 0; i < 6; i++)
		ASSERT_DOUBLE_EQ(ratedPower[i % 2], res[i]);
}

TEST_F(FastLoadReductionTest, flr_atRestoreTime) {
	double res[6];
	flr->calculatePowerAvailable(restoreTime, res);
	for (unsigned int i = 0; i < 6; i++)
		ASSERT_DOUBLE_EQ(powerAvailable1[i], res[i]);
}

TEST_F(FastLoadReductionTest, flr_atEndTime) {
	double res[6];
	flr->calculatePowerAvailable(restoreTime + rampTime, res);
	for (unsigned int i = 0; i < 6; i++)
		ASSERT_DOUBLE_EQ(ratedPower[i % 2], res[i]);
}

}
