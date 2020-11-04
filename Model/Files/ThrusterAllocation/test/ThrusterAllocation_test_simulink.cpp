/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "gtest/gtest.h"
#include "ThrustAllocation.h"

#define PI 3.141592653589793238462643383279

namespace {
class ThrusterAllocationTestSimulink: public ::testing::Test {
public:
	ThrusterAllocationTestSimulink() :
			::testing::Test(), nSwitchboards { 3 }, thruster2switchboard { 0, 0,
					1, 1, 2, 2 }, nThruster { 6 }, NSxPosition { -35, -35, 0, 0,
					35, 35 }, NSyPosition { -27, 27, -27, 27, -27, 27 }, NSTauMax {
					773146.60985993617, 773146.60985993617, 773146.60985993617,
					773146.60985993617, 773146.60985993617, 773146.60985993617 }, NSKt2p {
					0.0073549075905080955, 0.0073549075905080955,
					0.0073549075905080955, 0.0073549075905080955,
					0.0073549075905080955, 0.0073549075905080955 }, rampTime {
					10, 10, 10, 10, 10, 10 } {
	}
	;
protected:
	
	virtual void SetUp() {
		
		double fMin[] { -1, -1, -1, -1, -1, -1 };
		bool rotable[] {1,1,1,1,1,1};
		double turnTime[] { 60, 60, 60, 60, 60, 60 };
		double singularityCostGain = 1.0000000000000001e-05;
		double singularityCostSmoother = 1e-3;
		double reverseThrustePowerGain[] {1,1,1,1,1,1};
		double dfCost = 1;
		double dangleCost = 1e3;
		ta = new ThrustAllocation(nThruster, nSwitchboards, NSTauMax, NSKt2p,
				thruster2switchboard, NSxPosition, NSyPosition, rampTime,
				turnTime, fMin,rotable,reverseThrustePowerGain, 1.,singularityCostGain, singularityCostSmoother,dangleCost,dfCost);
	}
	virtual void TearDown() {
		delete ta;
	}
	const ushort nSwitchboards;

	const uint thruster2switchboard[6];
	const ushort nThruster;
	const double NSxPosition[6];
	const double NSyPosition[6];
	const double NSTauMax[6];
	const double NSKt2p[6];
	const double rampTime[6];

	ThrustAllocation* ta;
	void checkThrust(const double tau_d[], const double u[],
			const double alpha[], const double tol);
	double cost(const double tau_d[], const double u[], const double alpha[]);

	void checkOptimality(const double tau_d[], const double u[],
			const double alpha[], const double uExp[], const double alphaExp[],
			const double tol);

	void checkPowerConsumption(const double NSpowerAvailableBus[],
			const double NSpowerAvailableThruster[], const double u[],
			const uint switchboard2bus[], const uint nbus, const double tol =
					1e-4, const bool onConstraint = 0);
};

TEST_F(ThrusterAllocationTestSimulink, Initial) {
	const double NStau_d[] { 874819.9490554434, 0, 0 };
	const double NSpowerAvailableBus[] { 22235369.195121907 };
	const double NSpowerAvailableThruster[] { 5000000, 5000000, 5000000,
			5000000, 5000000, 5000000 };
	const uint switchboard2bus[] { 0, 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1, 1, 1 };
	double alphaOld[] { 3.9269908169872414, -0.78539816339744828,
			3.1415926535897931, 0, 2.3561944901923448, 0.78539816339744828 };
	double uOld[] { -0.20002387335257105, 0.20002387335257105,
			-0.28287647449360431, 0.28287647449360431, -0.20002387335257105,
			0.20002387335257105 };
	const double uExpected[] { -0.188584, 0.188584, -0.188584, 0.188584,
			-0.188584, 0.188584 };
	const double alphaExpected[] { 3.14159, 0, 3.14159, 0, 3.14159, 0 };
	double u[] { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };
	double alpha[] { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };
	ASSERT_TRUE(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, uOld, alphaOld, u,
					alpha));

	checkThrust(NStau_d, u, alpha, .0001);
	//checkOptimality(NStau_d, u, alpha, uExpected, alpha, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	//printUandAlpha(ta->getNThrusters(), u, uExpected, alpha, alphaExpected,
	//		1e-4, .1);
}



//TEST_F(ThrusterAllocationTestSimulink, ThrusterAllocation_Initial_Uold_exact) {
//	const double NStau_d[] { 874819.9490554434, 0, 0 };
//	const double NSpowerAvailableBus[] { 1e100 };
//	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100, 1e100,
//			1e100 };
//	const uint switchboard2bus[] { 0, 0, 0 };
//	const uint nbus = 1;
//	const bool thrustersOn[] { 1, 1, 1, 1, 1, 1 };
//	double alpha[] { 3.9269908169872414, -0.78539816339744828,
//			3.1415926535897931, 0, 2.3561944901923448, 0.78539816339744828 };
//	const double uExpected[] { -0.028117778104290, 0.028117778104290,
//			-0.039764543138884, 0.039764543138884, -0.028117778104290,
//			0.028117778104290 };
//	const double uOld[] { uExpected[0], uExpected[1], uExpected[2],
//			uExpected[3], uExpected[4], uExpected[5] };
//	double u[] { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };
//	ASSERT_TRUE(
//			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
//					switchboard2bus, nbus, thrustersOn, uOld, alpha, 0, u,
//					alpha));
//
//	checkThrust(NStau_d, u, alpha, .0001);
//	checkOptimality(NStau_d, u, alpha, uExpected, alpha, 1E-4);
//	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
//			switchboard2bus, nbus);
//	printUandAlpha(ta->getNThrusters(), u, uExpected, alpha, alpha, 1e-4, .1);
//}

//TEST_F(ThrusterAllocationTestSimulink, ThrusterAllocation_Initial_2DUMAX) {
//	const double NStau_d[] { 874819.9490554434, 0, 0 };
//	const double NSpowerAvailableBus[] { 1e100 };
//	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100, 1e100,
//			1e100 };
//	const uint switchboard2bus[] { 0, 0, 0 };
//	const uint nbus = 1;
//	const bool thrustersOn[] { 1, 1, 1, 1, 1, 1 };
//	double alpha[] { 3.9269908169872414, -0.78539816339744828,
//			3.1415926535897931, 0, 2.3561944901923448, 0.78539816339744828 };
//	const double uExpected[] { -0.028117778104290, 0.028117778104290,
//			-0.039764543138884, 0.039764543138884, -0.028117778104290,
//			0.028117778104290 };
//	const double uOld[] { uExpected[0], uExpected[1], uExpected[2],
//			uExpected[3], uExpected[4], uExpected[5] };
//	const double uUnconstrained[] { uExpected[0], uExpected[1], uExpected[2]
//			+ duMax[2] * 2, uExpected[3], uExpected[4], uExpected[5] };
//	double u[] { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };
//	ASSERT_TRUE(
//			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
//					switchboard2bus, nbus, thrustersOn, uOld, alpha,
//					uUnconstrained, u, alpha));
//
//	checkThrust(NStau_d, u, alpha, .0001);
//	checkOptimality(NStau_d, u, alpha, uExpected, alpha, 1E-4);
//	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
//			switchboard2bus, nbus);
//	printUandAlpha(ta->getNThrusters(), u, uExpected, alpha, alpha, 1e-4, .1);
//}

//TEST_F(ThrusterAllocationTestSimulink, ThrusterAllocation_Initial_Unconstrained_exact) {
//	const double NStau_d[] { 874819.9490554434, 0, 0 };
//	const double NSpowerAvailableBus[] { 1e100 };
//	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100, 1e100,
//			1e100 };
//	const uint switchboard2bus[] { 0, 0, 0 };
//	const uint nbus = 1;
//	const bool thrustersOn[] { 1, 1, 1, 1, 1, 1 };
//	double alpha[] { 3.9269908169872414, -0.78539816339744828,
//			3.1415926535897931, 0, 2.3561944901923448, 0.78539816339744828 };
//	const double uExpected[] { -0.028117778104290, 0.028117778104290,
//			-0.039764543138884, 0.039764543138884, -0.028117778104290,
//			0.028117778104290 };
//	const double uOld[] { uExpected[0], uExpected[1], uExpected[2],
//			uExpected[3], uExpected[4], uExpected[5] };
//	double u[] { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };
//	ASSERT_TRUE(
//			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
//					switchboard2bus, nbus, thrustersOn, uOld, alpha, uOld, u,
//					alpha));
//
//	checkThrust(NStau_d, u, alpha, .0001);
//	checkOptimality(NStau_d, u, alpha, uExpected, alpha, 1E-4);
//	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
//			switchboard2bus, nbus);
//	printUandAlpha(ta->getNThrusters(), u, uExpected, alpha, alpha, 1e-4, .1);
//}

//TEST_F(ThrusterAllocationTestSimulink, ThrusterAllocation_NO_Throw) {
//	const double NStau_d[] { 879482.61182011827, 1.672023080960795e-07,
//			1.4644506984440428e-05 };
//	const double NSpowerAvailableBus[] { 2568514.3607636942 };
//	const double NSpowerAvailableThruster[] { 6348780.6406300487,
//			6348780.6406304799, 6522822.7028749902, 6522822.7028749725,
//			6348780.6406475836, 6348780.640630668 };
//	const uint switchboard2bus[] { 0, 0, 0 };
//	const uint nbus = 1;
//	const bool thrustersOn[] { 1, 1, 1, 1, 1, 1 };
//	double alpha[] { 3.9269908169872414, -0.78539816339744828,
//			3.1415926535897931, 0, 2.3561944901923448, 0.78539816339744828 };
//	const double uExpected[] { -0.0198831, 0.0198831, -0.0397661, 0.0397661,
//			-0.0198831, 0.0198831 };
//	const double uOld[] { -0.021181803858358751, 0.021181803851851106,
//			-0.038942104661707069, 0.03894210455596573, -0.021181803955349666,
//			0.021181803948799684 };
//	const double uUnconstrained[] { -0.028267641761530685, 0.028267641761529488,
//			-0.039976482355461175, 0.039976482355458677, -0.028267641761509171,
//			0.028267641761550985 };
//	double u[] { -0.028267641761530685, 0.028267641761529488,
//			-0.039976482355461175, 0.039976482355458677, -0.028267641761509171,
//			0.028267641761550985 };
//	ASSERT_TRUE(
//			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
//					switchboard2bus, nbus, thrustersOn, uOld, alpha, uUnconstrained, u,
//					alpha));
//
//	//checkThrust(NStau_d, u, alpha, .0001);
//	checkOptimality(NStau_d, u, alpha, uExpected, alpha, 1E-4);
//	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
//			switchboard2bus, nbus);
//	printUandAlpha(ta->getNThrusters(), u, uExpected, alpha, alpha, 1e-4, .1);
//}

}
void ThrusterAllocationTestSimulink::checkThrust(const double tau_d[],
		const double u[], const double alpha[], const double tol) {
	double tauX, tauY, tauN;
	tauX = tauY = tauN = 0;
	for (uint i = 0; i < nThruster; i++) {
		tauX += NSTauMax[i] * u[i] * cos(alpha[i]);
		tauY += NSTauMax[i] * u[i] * sin(alpha[i]);
		tauN += -NSTauMax[i] * u[i] * cos(alpha[i]) * NSyPosition[i]
				+ NSTauMax[i] * u[i] * sin(alpha[i]) * NSxPosition[i];
	}
	ASSERT_NEAR(tauX, tau_d[0], tol + tol * fabs(tau_d[0]));
	ASSERT_NEAR(tauY, tau_d[1], tol + tol * fabs(tau_d[1]));
	ASSERT_NEAR(tauN, tau_d[2], tol + tol * fabs(tau_d[2]));
}

void ThrusterAllocationTestSimulink::checkOptimality(const double tau_d[],
		const double u[], const double alpha[], const double uExp[],
		const double alphaExp[], const double tol) {
	double expectedCost = cost(tau_d, uExp, alphaExp);
	double costNew = cost(tau_d, u, alpha);
	if (costNew < expectedCost)
		cout << "NEW SOLUTION IS BETTER THAN EXPECTED" << endl;
	ASSERT_NEAR(costNew, expectedCost, costNew * tol);
	
}

double ThrusterAllocationTestSimulink::cost(const double tau_d[],
		const double u[], const double alpha[]) {
	double cost = 0;
	for (uint i = 0; i < nThruster; i++)
		cost += pow(NSTauMax[i] * u[i], 2) * NSKt2p[i];
	return cost;
}

void ThrusterAllocationTestSimulink::checkPowerConsumption(
		const double NSpowerAvailableBus[],
		const double NSpowerAvailableThruster[], const double u[],
		const uint switchboard2bus[], const uint nbus, const double tol,
		const bool onConstraint) {
	double* busPower = new double[nbus];
	for (uint i = 0; i < nbus; i++)
		busPower[i] = 0;
	
	for (uint i = 0; i < nThruster; i++) {
		double power = NSKt2p[i] * pow(fabs(NSTauMax[i] * u[i]), 1.5);
		ASSERT_LE(power * (1 - tol), NSpowerAvailableThruster[i]);
		busPower[switchboard2bus[thruster2switchboard[i]]] += power;
	}
	
	for (uint i = 0; i < nbus; i++) {
		ASSERT_LE(busPower[i] * (1 - tol), NSpowerAvailableBus[i]);
		if (onConstraint)
			ASSERT_GE(busPower[i] * (1 + tol), NSpowerAvailableBus[i]);
	}
	
	delete[] busPower;
}

