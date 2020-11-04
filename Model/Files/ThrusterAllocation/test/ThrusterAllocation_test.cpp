/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "gtest/gtest.h"
#include "../src/ThrustAllocation.h"

#define PI 3.141592653589793238462643383279
void printUandAlpha(const uint nThrusters, const double u[],
		const double uExpected[], const double alpha[],
		const double alphaExpected[], const double tolU,
		const double tolAlpha) {
	bool exceedTol = false;
	for (uint i = 0; i < nThrusters; i++) {
		if (fabs(u[i] - uExpected[i]) > tolU) {
			exceedTol = true;
			break;
		}
		if (fabs(alpha[i] - alphaExpected[i]) > tolAlpha) {
			exceedTol = true;
			break;
		}
	}
	if (!exceedTol)
		return;

	for (uint i = 0; i < nThrusters; i++) {
		cout << i << " u: " << u[i] << " \tuExp: " << uExpected[i]
				<< " \talpha: " << alpha[i] << " \talphaExp: "
				<< alphaExpected[i] << endl;
	}
	for (uint i = 0; i < 4; i++) {
		cout << i << endl;
		ASSERT_NEAR(alphaExpected[i], alpha[i], tolAlpha);
		ASSERT_NEAR(uExpected[i], u[i], tolU);
	}

}

namespace {
class ThrusterAllocationTest: public ::testing::Test {
public:
	ThrusterAllocationTest() :
			::testing::Test(), nSwitchboards { 2 }, thruster2switchboard { 0, 0,
					1, 1 }, nThruster { 4 }, NSxPosition { 35, 30, -35, -35 }, NSyPosition {
					0, 0, -5, 5 }, NSTauMax { 92.0394e3, 92.0394e3, 184.2016e3,
					184.2016e3 }, NSKt2p { 2.5e9 / powf(NSTauMax[0], 1.5), 2.5e9
					/ powf(NSTauMax[1], 1.5), 2.5e9 / powf(NSTauMax[2], 1.5),
					2.5e9 / powf(NSTauMax[3], 1.5) }, turnTime { 60, 60, 60, 60 }, rampTime {
					5, 5, 5, 5 } {
	}
	;
protected:

	virtual void SetUp() {

		double fMin[] { -1, -1, -1, -1 };
		bool rotable[] { 1, 1, 1, 1, 1, 1 };
		double reverseThrustePowerGain[] { 1, 1, 1, 1, 1, 1 };
		double dfCost = 1e-3;
		double dangleCost = 1e-3;
		ta = new ThrustAllocation(nThruster, nSwitchboards, NSTauMax, NSKt2p,
				thruster2switchboard, NSxPosition, NSyPosition, rampTime,
				turnTime, fMin, rotable, reverseThrustePowerGain, 1., 0., 0.,dangleCost,dfCost);
	}
	virtual void TearDown() {
		delete ta;
	}
	const ushort nSwitchboards;
	const uint thruster2switchboard[4];
	const ushort nThruster;
	const double NSxPosition[4];
	const double NSyPosition[4];
	const double NSTauMax[4];
	const double NSKt2p[4];
	const double turnTime[4];
	double rampTime[4];

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

TEST_F(ThrusterAllocationTest, ThrusterAllocation_Yaw) {
	const double NStau_d[] { 0, 0, 100e3 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	const double alphaExpected[] { 1.570796326794897, 1.5708, -1.21574, -1.92585 };
	const double uExpected[] { 0.00818185, 0.00745797, 0.00416727, 0.00416727 };
	double u[4] { uExpected[0], uExpected[1], uExpected[2], uExpected[3] };
	double alpha[4] { alphaExpected[0], alphaExpected[1], alphaExpected[2],
			alphaExpected[3] };
	for (uint i = 0; i < 1; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}
	checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, .1);
}

TEST_F(ThrusterAllocationTest, NoPower) {
	const double NStau_d[] { 1E3, 0, 0 };
	const double NSpowerAvailableBus[] { 0, 0 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 1 };
	const uint nbus = 2;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	const double alphaExpected[] { 1.570796326794897, 1.5708, -1.21574, -1.92585 };
	const double uExpected[] { 0, 0, 0, 0 };
	double u[4] { uExpected[0], uExpected[1], uExpected[2], uExpected[3] };
	double alpha[4] { alphaExpected[0], alphaExpected[1], alphaExpected[2],
			alphaExpected[3] };

	ASSERT_TRUE(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, u, alpha, u, alpha));

	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, .1);
}

TEST_F(ThrusterAllocationTest, PowerOneBus) {
	const double NStau_d[] { 0, 1E3, 0 };
	const double NSpowerAvailableBus[] { 1E100, 0 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 1 };
	const uint nbus = 2;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	const double alphaExpected[] { -1.570796326794897, 1.5708, -1.21574,
			-1.92585 };
	const double uExpected[] { 0.0651895, 0.0760544, 0, 0 };
	double u[4] { uExpected[0], uExpected[1], uExpected[2], uExpected[3] };
	double alpha[4] { alphaExpected[0], alphaExpected[1], alphaExpected[2],
			alphaExpected[3] };

	ASSERT_TRUE(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, u, alpha, u, alpha));

	checkThrust(NStau_d, u, alpha, .1);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, .1);
}

TEST_F(ThrusterAllocationTest, NoPowerToThrusters) {
	const double NStau_d[] { 0, 1E3, 0 };
	const double NSpowerAvailableBus[] { 1E100, 1E100 };
	const double NSpowerAvailableThruster[] { 0, 0, 0, 0 };
	const uint switchboard2bus[] { 0, 1 };
	const uint nbus = 2;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	const double alphaExpected[] { 1.570796326794897, 1.5708, -1.21574, -1.92585 };
	const double uExpected[] { 0, 0, 0, 0 };
	double u[4] { uExpected[0], uExpected[1], uExpected[2], uExpected[3] };
	double alpha[4] { alphaExpected[0], alphaExpected[1], alphaExpected[2],
			alphaExpected[3] };

	ASSERT_TRUE(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, u, alpha, u, alpha));
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, .1);
}

TEST_F(ThrusterAllocationTest,Surge) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double u[4] { 0, 0, 0, 0 };
	double alpha[4] { PI / 4, -PI / 4, PI / 4, -PI / 4 };
	for (uint i = 0; i < 200; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	const double alphaExpected[] { 0, 0, 0, 0 };
	const double uExpected[] { 0.108533, 0.108533, 0.217211, 0.217211 };

	checkThrust(NStau_d, u, alpha, .0001);
	//checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-3);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	//printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, 1e-2);

}

TEST_F(ThrusterAllocationTest, Surge_Correct_Initials) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double alpha[] { 0, 0, 0, 0 };
	const double alphaExpected[] { 0, 0, 0, 0 };
	const double uExpected[] { 0.108533, 0.108533, 0.217211, 0.217211 };
	double u[] { 0.108533, 0.108533, 0.217211, 0.217211 };

	ASSERT_TRUE(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, u, alpha, u, alpha));

	checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, 1e-2);

}

TEST_F(ThrusterAllocationTest, Surge_Correct_Initials_WrongDirection) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double u[4] { 0, 0, -.0001, 0 };
	double alpha[4] { 0, 0, 0, 0 };
	for (uint i = 0; i < 10; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	const double alphaExpected[] { 0, 0, 0, 0 };
	const double uExpected[] { 0.130802, 0.130802, 0.206052, 0.206117 };

	checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, 1e-2);

}

TEST_F(ThrusterAllocationTest, Surge_Initial_All_Wrong_Direction) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double u[4] { .01, .01, .01, .01 };
	double alpha[4] { PI, PI, PI, PI };
	for (uint i = 0; i < 1; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	const double alphaExpected[] { PI, PI, PI, PI };
	const double uExpected[] { -0.162992, -0.162992, -0.19, -0.19 };

	checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, 1e-2);

}

TEST_F(ThrusterAllocationTest, Surge_dalphaConstrain) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double u[4] { 0, 0, 0, 0 };
	double dAlphaMax[4] = { 2 * PI / turnTime[0], 2 * PI / turnTime[1], 2 * PI
			/ turnTime[2], 2 * PI / turnTime[3] };
	double alpha[4] { 2 * (2 * PI / turnTime[0]), 0, 0, 0 };
	ASSERT_TRUE(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, u, alpha, u, alpha));

	ASSERT_GE(alpha[0], dAlphaMax[0] * (1 - 1e-6));
	ASSERT_GE((3 + 1e-6) * dAlphaMax[0], alpha[0]);
	for (uint i = 1; i < 4; i++) {
		ASSERT_GE(alpha[i], -dAlphaMax[i] * (1 + 1e-6));
		ASSERT_GE(dAlphaMax[i] * (1 + 1e-6), alpha[i]);
	}
	checkThrust(NStau_d, u, alpha, .0001);

}

TEST_F(ThrusterAllocationTest, ThrusterAllocation_ONE_OFF) {
	const double NStau_d[] { 100e2, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 0, 1 };
	double u[4] { 0, 0, 0, 0 };
	double alpha[4] { 0, 0, 0, 0 };
	ASSERT_NO_THROW(
			ta->thrust(NStau_d, NSpowerAvailableBus, NSpowerAvailableThruster,
					switchboard2bus, nbus, thrustersOn, u, alpha, u, alpha));

	const double alphaExpected[] { 0.10472, 0.10472, 0, -0.073921 };
	const double uExpected[] { 0.0227281, 0.0224294, 0, 0.0319354 };

	checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, 1e-2);

}

TEST_F(ThrusterAllocationTest, ThrusterAllocation_Sway) {
	const double NStau_d[] { 0, 100e3, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };

	const double alphaExpected[] { 1.570796326794897, 1.570726916939863,
			1.341262552353169, 1.800446700349857 };
	const double uExpected[] { 0.284491861614177, 0.269049483277609,
			0.136736935419962, 0.136709874326808 };
	double u[4] { uExpected[0], uExpected[1], uExpected[2], uExpected[3] };
	double alpha[4] { alphaExpected[0], alphaExpected[1], alphaExpected[2],
			alphaExpected[3] };
	for (uint i = 0; i < 10; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-3);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	//printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, 1e-3);
}

TEST_F(ThrusterAllocationTest, ThrusterAllocation_Yaw_badInit) {
	const double NStau_d[] { 0, 0, 100e3 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double u[4] { .001, .001, .001, .001 };
	double alpha[] = { 0, PI / 2, PI, 3 / 2 * PI };
	for (uint i = 0; i < 50; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
		if (i > 3) {
			checkThrust(NStau_d, u, alpha, .0001);
			checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster,
					u, switchboard2bus, nbus);
		}
	}
}

TEST_F(ThrusterAllocationTest, Power_ThrusterReduced) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e100 };
	const double NSpowerAvailableThruster[] { 99990, 99991, 99992, 99993 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double alpha[] { 1.570796326794897, 1.570726916939863, 1.341262552353169,
			1.800446700349857 };
	double u[] { 0.284491861614177, 0.269049483277609, 0.136736935419962,
			0.136709874326808 };
	//ta->setPrintLevel(PL_LOW);
	ta->initialize(u,alpha);
	for (uint i = 0; i < 1; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
}

TEST_F(ThrusterAllocationTest, ThrusterAllocation_Power_BusReduced) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 1e4, 2e4 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 1 };
	const uint nbus = 2;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double alpha[] { 1.570796326794897, 1.570726916939863, 1.341262552353169,
			1.800446700349857 };
	double u[] { 0.284491861614177, 0.269049483277609, 0.136736935419962,
			0.136709874326808 };
	//ta->setPrintLevel(PL_HIGH);
	for (uint i = 0; i < 1; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus, 2e-2, 0);
}

TEST_F(ThrusterAllocationTest, ThrusterAllocation_Power_BusReduced_ClosedBus) {
	const double NStau_d[] { 100e3, 0, 0 };
	const double NSpowerAvailableBus[] { 3e6 };
	const double NSpowerAvailableThruster[] { 1e100, 1e100, 1e100, 1e100 };
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1 };
	double alpha[] { 0, 0, 0, 0 };
	double u[] { 0.284491861614177, 0.269049483277609, 0.136736935419962,
			0.136709874326808 };
	for (uint i = 0; i < 1; i++) {
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));
	}

	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus, 1e-3, 1);
}

}

void ThrusterAllocationTest::checkThrust(const double tau_d[], const double u[],
		const double alpha[], const double tol) {
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

void ThrusterAllocationTest::checkOptimality(const double tau_d[],
		const double u[], const double alpha[], const double uExp[],
		const double alphaExp[], const double tol) {
	double expectedCost = cost(tau_d, uExp, alphaExp);
	double costNew = cost(tau_d, u, alpha);
	if (costNew < expectedCost)
		cout << "NEW SOLUTION IS BETTER THAN EXPECTED" << endl;
	ASSERT_NEAR(costNew, expectedCost, costNew * tol);

}

double ThrusterAllocationTest::cost(const double tau_d[], const double u[],
		const double alpha[]) {
	double cost = 0;
	for (uint i = 0; i < nThruster; i++)
		cost += pow(NSTauMax[i] * u[i], 2) * NSKt2p[i];
	return cost;
}

void ThrusterAllocationTest::checkPowerConsumption(
		const double NSpowerAvailableBus[],
		const double NSpowerAvailableThruster[], const double u[],
		const uint switchboard2bus[], const uint nbus, const double tol,
		const bool onConstraint) {
	vector<double> busPower(nbus);
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
}
