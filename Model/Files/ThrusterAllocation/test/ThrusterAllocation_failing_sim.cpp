/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "ThrustAllocation.h"

namespace {
class ThrusterAllocationTestFailing: public ::testing::Test {
public:

protected:

	virtual void SetUp() {

		double fMin[] { -1, -1, -1, -1, -1, -1 };
		bool rotable[] { 1, 1, 1, 1, 1, 1 };
		double reverseThrustePowerGain[] { 1.3694, 1.3694,1.3694,1.3694,1.3694,1.3694};
		double singularityAvoidanceCostSmoother = 1e-4;
		double singularityAvoidanceCostGain = 5;
	    const ushort nSwitchboards = 3;
	    const uint thruster2switchboard[6] = {0,0,1,1,2,2};
	    const ushort nThruster = 6;
	    const double NSxPosition[6] = { -35, -35, 0, 0, 35, 35 };
	    const double NSyPosition[6] = { -27,27,-27,27,-27,27};
	    const double NSTauMax[6] = {5.062663867171433e+05,5.062663867171433e+05,5.062663867171433e+05,5.062663867171433e+05,5.062663867171433e+05,5.062663867171433e+05};
	    const double NSKt2p[6] = {0.010918083304765,0.010918083304765,0.010918083304765,0.010918083304765,0.010918083304765,0.010918083304765};
	    const double turnTime[6] = {60,60,60,60,60,60};
	    double rampTime[6] = {5.,5.,5.,5.,5.,5.};
		double dfCost = 1;
		double dangleCost = 1e3;


		ta = new ThrustAllocation(nThruster, nSwitchboards, NSTauMax, NSKt2p,
				thruster2switchboard, NSxPosition, NSyPosition, rampTime,
				turnTime, fMin, rotable, reverseThrustePowerGain, 1., singularityAvoidanceCostGain, singularityAvoidanceCostSmoother,dangleCost,dfCost);
	}
	virtual void TearDown() {
		delete ta;
	}

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

TEST_F(ThrusterAllocationTestFailing, ThrusterAllocation_Yaw) {
	const double NStau_d[] { 675015.9399,-6420.9329,-583156.0815};
	const double NSpowerAvailableBus[] { 22078493.52, 0, 0};
	const double NSpowerAvailableThruster[] { 5e+06,5e+06,5e+06,5e+06,5e+06,5e+06};
	const uint switchboard2bus[] { 0, 0 };
	const uint nbus = 1;
	const bool thrustersOn[] { 1, 1, 1, 1,1,1 };
	double u[6] { -0.9074386736, 0.0002339994115, -0.9097303142, -0.4234073408, -0.9074757333, 8.059212402e-06};
	double alpha[6] { 4.245784108, -1.583830597, 3.143716099, 0.02896169799, 2.039508678, 1.743077249};
		ASSERT_TRUE(
				ta->thrust(NStau_d, NSpowerAvailableBus,
						NSpowerAvailableThruster, switchboard2bus, nbus,
						thrustersOn, u, alpha, u, alpha));

	ta->dump();


	/*checkThrust(NStau_d, u, alpha, .0001);
	checkOptimality(NStau_d, u, alpha, uExpected, alphaExpected, 1E-4);
	checkPowerConsumption(NSpowerAvailableBus, NSpowerAvailableThruster, u,
			switchboard2bus, nbus);
	printUandAlpha(4, u, uExpected, alpha, alphaExpected, 1e-4, .1);
	*/
}


}
/*
void ThrusterAllocationTestFailing::checkThrust(const double tau_d[], const double u[],
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

void ThrusterAllocationTestFailing::checkOptimality(const double tau_d[],
		const double u[], const double alpha[], const double uExp[],
		const double alphaExp[], const double tol) {
	double expectedCost = cost(tau_d, uExp, alphaExp);
	double costNew = cost(tau_d, u, alpha);
	if (costNew < expectedCost)
		cout << "NEW SOLUTION IS BETTER THAN EXPECTED" << endl;
	ASSERT_NEAR(costNew, expectedCost, costNew * tol);

}

double ThrusterAllocationTestFailing::cost(const double tau_d[], const double u[],
		const double alpha[]) {
	double cost = 0;
	for (uint i = 0; i < nThruster; i++)
		cost += pow(NSTauMax[i] * u[i], 2) * NSKt2p[i];
	return cost;
}

void ThrusterAllocationTestFailing::checkPowerConsumption(
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
*/
