/*
 * ThrustAllocation.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 7, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 *
 *  All parameters with prefix NS are not scaled/normalized.
 */

#include "ThrustAllocation.h"
#include <ctime>
#include <iostream>

template<typename T>
int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

#define PI 3.141592653589793238462643383279

const double ThrustAllocation::TAU_BASE = 1.e6;
const double ThrustAllocation::POWER_BASE = 1.e8;
const double ThrustAllocation::LENGTH_BASE = 100.;
const double ThrustAllocation::FMAX = 1.;

ThrustAllocation::ThrustAllocation(const uint nThruster,
		const uint nSwitchboards, const double NSTauMax[],
		const double NSKt2p[], const uint thruster2switchboard[],
		const double NSxPosition[], const double NSyPosition[],
		const double fRampTime[], const double turnTime[], const double fMin[],
		const bool rotable[], const double reverseThrustPowerGain[],
		const double samplingTime, const double singularityCostGain,
		const double singularityCostSmoother, const double dAngleCost, const double dfCost) :
		_nthrusters(nThruster), _nswitchboards(nSwitchboards), _xPosition(
				NSxPosition, NSxPosition + nThruster), _yPosition(NSyPosition,
				NSyPosition + nThruster), _fMin(fMin, fMin + nThruster), _rotable(
				nThruster), _thruster2switchboard(thruster2switchboard,
				thruster2switchboard + nThruster), _reverseThrustPowerGain(
				reverseThrustPowerGain, reverseThrustPowerGain + nThruster), _sampleTime(
				samplingTime), _pLast(nThruster * 3 + 2 * DOF), _singularityCostGain(
				singularityCostGain), _singularityCostSmoother(
				singularityCostSmoother), _singularityCostWrapper(
				getNThrusters(), singularityCostSmoother, singularityCostGain), _printLevel(
				PL_NONE), _dAngleCost(dAngleCost), _dfCost(dfCost) {
	_nbus = 0;
	_pLast.setZero();
	_thrustersOn = new bool[getNThrusters()];
	_dfMax = vector<double>(getNThrusters());
	_dAlphaMax = vector<double>(getNThrusters());
	for (unsigned int i = 0; i < getNThrusters(); i++) {
		_dfMax[i] = samplingTime / fRampTime[i];
		_dAlphaMax[i] = 2 * PI / turnTime[i] * samplingTime;
		_rotable[i] = rotable[i];
	}
	ASSERT(DOF == 3,"It is assumed that the degrees of freedom are surge, sway, and yaw");

	// Check that input is valid
	for (unsigned int i = 0; i < getNThrusters(); i++) {
		ASSERT(NSTauMax[i] > 0, "TauMax must be positive");
		ASSERT(
				thruster2switchboard[i] >= 0
						&& thruster2switchboard[i] < nSwitchboards,
				"thruster2switchboard must be between 0 and nSwitchboards-1");
		ASSERT(fRampTime[i] > 0, "fRampTime must be positive");
		ASSERT(turnTime[i] > 0, "turnTime must be positive");
	}
	ASSERT(samplingTime > 0, "Sample time must be positive");
	ASSERT(singularityCostGain >= 0,
			"Singularity avoidance cost gain must be non-negative");
	ASSERT(singularityCostSmoother >= 0,
			"Singularity avoidance cost smoother must be non-negative");

	// Scale input
	setKu2pK(NSKt2p, NSTauMax);
	scaleLength(getNThrusters(), &_xPosition[0]);
	scaleLength(getNThrusters(), &_yPosition[0]);
}

ThrustAllocation::~ThrustAllocation() {
  delete[] _thrustersOn;
}

void ThrustAllocation::setKu2pK(const double NSKt2p[], const double NSK[]) {
	_Ku2p = vector<double>(NSKt2p, NSKt2p + getNThrusters());
	scaleKt2p(getNThrusters(), &_Ku2p[0]);

	_thrustMax = vector<double>(NSK, NSK + getNThrusters());
	scaleThrust(getNThrusters(), &_thrustMax[0]);

	for (uint i = 0; i < _nthrusters; i++) {
		_Ku2p[i] *= pow(_thrustMax[i], 1.5);
	}
}

void ThrustAllocation::convertFtoU(const double f[], const double alpha[],
		double ux[], double uy[]) const {
	for (uint i = 0; i < getNThrusters(); i++) {
		ux[i] = f[i] * cos(alpha[i]);
		uy[i] = f[i] * sin(alpha[i]);
	}
}

bool ThrustAllocation::thrust(const double NStau_d[],
		const double NSpowerAvailableBus[],
		const double NSpowerAvailableThruster[], const uint switchboard2bus[],
		const uint nbus, const bool thrustersOnUnchecked[], const double fOldIn[],
		const double alphaOld[], double fOut[], double alphaOut[],
		double powerAllocatedThruster[]) {

	vector<double> fOld(getNThrusters());
	for (uint i=0; i<getNThrusters(); i++) {
		double f = fOldIn[i];
		if (f >= 0 && f < 1e-5) {
			fOld[i] = 1e-5;
		} else if (f < 0 && f > -1e-5) {
			fOld[i] = -1e-5;
		} else {
			fOld[i] = f;
		}
	}
	initialize(&fOld[0],alphaOld);
  // Check that input is valid
	for (unsigned int i = 0; i < nbus; i++) {
		ASSERT(NSpowerAvailableBus[i] >= 0,
				"PowerAvailableBus must be non-negative");
	}
	for (unsigned int i = 0; i < getNThrusters(); i++) {
		ASSERT(NSpowerAvailableThruster[i] >= 0,
				"PowerAvailableThruster must be non-negative");
	}
	for (unsigned int i = 0; i < getNSwitchboard(); i++) {
		ASSERT(switchboard2bus[i] >= 0 && switchboard2bus[i] < nbus,
				"switchboard2bus must be between 0 and nbus");
	}

    setPlantConfiguration(switchboard2bus, nbus);

	vector<double> alpha(getNThrusters());
	vector<double> f(getNThrusters());
	vector<double> tau_d(NStau_d, NStau_d + DOF);
	vector<double> powerAvailableBus(NSpowerAvailableBus,
			NSpowerAvailableBus + nbus);
	vector<double> powerAvailableThruster(NSpowerAvailableThruster,
			NSpowerAvailableThruster + getNThrusters());

	// Scale input
	scaleTau(&tau_d[0]);
	scalePower(nbus, &(powerAvailableBus[0]));
	scalePower(getNThrusters(), &powerAvailableThruster[0]);
	// Find current direction list
	vector<signed char> directionList = findDirection0(&fOld[0]);
	vector<double> uxOld(getNThrusters());
	vector<double> uyOld(getNThrusters());

	convertFtoU(&fOld[0], alphaOld, &uxOld[0], &uyOld[0]);

    /// Check if thrusters have power
    setThrustersOn(&powerAvailableBus[0],
            thrustersOnUnchecked);

	CFunction cfun(1, 0);
	if (_singularityCostGain > 0) {
		cfun = _singularityCostWrapper.getCFunction(_thrustersOn);
	}



    /// Check if any thruster is connected
    if (!isAnyThrusterConnected()) {
        for (uint i = 0; i < getNThrusters(); i++) {
            fOut[i] = 0;
            alphaOut[i] = alphaOld[i];
        }
        return true;

    }

	// Calculate optimal thrust demand with current direction list
	double cost;
	DVector pBest(nOptVariables());
	returnValue ret = solve(&tau_d[0], &powerAvailableBus[0],
			&powerAvailableThruster[0], cfun, &fOld[0], alphaOld,
			&directionList[0],false, &f[0], &alpha[0], cost, pBest);


	if (ret != SUCCESSFUL_RETURN) {
	    std::cout << "Problem solving TA with azimuth angles, freezes angles." << endl;
	    ret = solve(&tau_d[0], &powerAvailableBus[0],
	            &powerAvailableThruster[0], cfun, &fOld[0], alphaOld,
	            &directionList[0], true, &f[0], &alpha[0], cost, pBest);

	    if (ret != SUCCESSFUL_RETURN) {
	        ret.print();
	        return false;
	    }
	    // Find best solution by checking other possible direction list
        findBestDirection(&tau_d[0], &powerAvailableBus[0],
                &powerAvailableThruster[0], cfun, &fOld[0], alphaOld,
                &f[0], &alpha[0], &directionList[0], cost, 0, true, &f[0], &alpha[0],
                cost, pBest);
	} else {

      // Find best solution by checking other possible direction list
      findBestDirection(&tau_d[0], &powerAvailableBus[0],
              &powerAvailableThruster[0], cfun, &fOld[0], alphaOld,
              &f[0], &alpha[0], &directionList[0], cost, 0, false, &f[0], &alpha[0],
              cost, pBest);
	}

	// Prepare output
	for (uint i = 0; i < getNThrusters(); i++) {
		fOut[i] = f[i];
		alphaOut[i] = alpha[i];
	}
	_pLast = pBest;
	if (powerAllocatedThruster != 0) {
		powerAllocation(fOut, &powerAvailableBus[0], powerAllocatedThruster);

		unScalePower(getNThrusters(), powerAllocatedThruster);

	}

	return true;

}

void ThrustAllocation::powerAllocation(const double f[],
		const double powerAvailableBus[], double powerThrusters[]) const {
	vector<double> powerConsumptionBus(_nbus, 0.);
	vector<double> powerConsumptionThruster(getNThrusters(), 0.);
	vector<uint> nThrusterPerBus(_nbus, 0);

	// Calculate power demand of each bus
	for (unsigned int i = 0; i < getNThrusters(); i++) {
        if (_thrustersOn[i]) {
		signed char direction = (f[i] >= 0) * 2 - 1;
		powerConsumptionThruster[i] = getKu2p(i, direction)
				* pow(fabs(f[i]), 1.5);
		powerConsumptionBus[thruster2bus(i)] += powerConsumptionThruster[i];
		nThrusterPerBus[thruster2bus(i)]++;
        }
	}

	// Calculate excessive power available
	vector<double> excessPowerBusPerThruster(_nbus);
	for (unsigned int i = 0; i < _nbus; i++) {
		excessPowerBusPerThruster[i] = (powerAvailableBus[i]
				- powerConsumptionBus[i]) / nThrusterPerBus[i];
	}

	// Allocates power to each thruster (power needed for thrust demand + excessive power per thruster
	for (uint i = 0; i < getNThrusters(); i++) {
        if (_thrustersOn[i]) {
            powerThrusters[i] = powerConsumptionThruster[i]
            + excessPowerBusPerThruster[thruster2bus(i)];
        } else {
            powerThrusters[i] = 0;
        }

	}

}

void ThrustAllocation::findBestDirection(const double tau_d[],
		const double powerAvailableBus[], const double powerAvailableThruster[],
		CFunction &singularityCost,
		const double fOld[], const double alphaOld[], const double fCurrent[],
		const double alphaCurrent[], const signed char directionListCurrent[],
		const double costCurrent, const uint iCurrent, bool fixThrusterAngles, double fBest[],
		double alphaBest[], double &costBest, DVector &pBest) const {
	double fMax = fabs(fCurrent[0]);
	for (uint i = 1; i < getNThrusters(); i++) {
		if (fMax < fabs(fCurrent[i]))
			fMax = fabs(fCurrent[i]);
	}

	costBest = costCurrent;

	for (uint i = iCurrent; i < getNThrusters(); i++) {
		double fMaxI = min(
				min(1.,
						pow(
								powerAvailableThruster[i]
										/ getKu2p(i, directionListCurrent[i]),
								2. / 3.)), fOld[i] + _dfMax[i]);
		// Check if the thrust is low compared with maximum thrust
		// and this level is not constrained by maximum allowed thrust set by power constraint
		bool isThrustLow1 = fabs(fCurrent[i]) < fMax * 1e-3
				&& fMax * 1e-3 < fMaxI;
		// Check if thrust is low and this level is not constrained
		// by maximum allowed thrust set by power constraint
		bool isThrustLow2 = (fabs(fCurrent[i]) < 1e-6 && 1e-6 < fMaxI);
		if (isThrustLow1 || isThrustLow2) {
		    // If so copy input directionList and switch direction of this thruster
			vector<signed char> directionList(directionListCurrent,
					directionListCurrent + getNThrusters());
			directionList[i] *= -1;

			double cost;
			DVector p(nOptVariables());
			vector<double> f(getNThrusters());
			vector<double> alpha(getNThrusters());
			returnValue ret = solve(tau_d, powerAvailableBus,
					powerAvailableThruster, singularityCost, fOld,
					alphaOld, &directionList[0], fixThrusterAngles, &f[0], &alpha[0], cost, p);

			double tol = 1e-4;
			if (ret == SUCCESSFUL_RETURN && cost <= costBest * (1 + tol)) {
			    // If solution is better than or close to better than earlier solution
				if (cost < costBest) {
				    // If better than earlier solution set this as best solution
					for (unsigned i = 0; i < getNThrusters(); i++) {
						alphaBest[i] = alpha[i];
						fBest[i] = f[i];
					}
					for (unsigned i = 0; i < nOptVariables(); i++)
						pBest(i) = p(i);
					costBest = cost;
				}
				// Check if there exists a better solution with the new solution as basis
				findBestDirection(tau_d, powerAvailableBus,
						powerAvailableThruster, singularityCost,
						fOld, alphaOld, &f[0], &alpha[0], &directionList[0],
						cost, i + 1,fixThrusterAngles, fBest, alphaBest, costBest, p);
			}
		}
	}
}

unsigned int ThrustAllocation::nOptVariables() const {
	return getNThrusters() * 2 + 3 * DOF;
}

void ThrustAllocation::printDirectionList(
		const signed char directionList[]) const {
	cout << "Direction list: ";
	for (unsigned int i = 0; i < getNThrusters(); i++){
		cout << int(directionList[i]>0);
	}
	cout << endl;
}

double ThrustAllocation::getKu2p(int thrusterI, signed char direction) const {
	if (direction > 0)
		return _Ku2p[thrusterI];
	else
		return _Ku2p[thrusterI] * _reverseThrustPowerGain[thrusterI];
}

returnValue ThrustAllocation::solve(const double tau_d[],
		const double powerAvailableBus[], const double powerAvailableThruster[],
		CFunction &singularityCostFun,
		const double fOld[], const double alphaOld[],
		const signed char directionList[], const bool fixThrusterAngles, double fOut[], double alphaOut[],
		double &cost, DVector &pResult) const {

	vector<double> uxOld(getNThrusters());
	vector<double> uyOld(getNThrusters());
	convertFtoU(fOld, alphaOld, &uxOld[0], &uyOld[0]);

	vector<Parameter> ux;
	vector<Parameter> uy;
	ux.reserve(getNThrusters());
	uy.reserve(getNThrusters());
	for (unsigned i = 0; i < getNThrusters(); i++) {
		ux.push_back(Parameter());
		uy.push_back(Parameter());
	}

// Slack variable for thrust positive and negative
	vector<Parameter> sTauP;
	sTauP.reserve(DOF);
	for (unsigned i = 0; i < DOF; i++)
		sTauP.push_back(Parameter());
	vector<Parameter> sTauN;
	sTauN.reserve(DOF);
	for (unsigned i = 0; i < DOF; i++)
		sTauN.push_back(Parameter());

// Slack variable for reducing thrust faster than possible
	vector<Parameter> sdf;
	sdf.reserve(getNThrusters());
	for (unsigned i = 0; i < getNThrusters(); i++)
		sdf.push_back(Parameter());

	Expression tauX = 0;
	Expression tauY = 0;
	Expression tauN = 0;
	for (uint i = 0; i < getNThrusters(); i++) {
		tauX = tauX + _thrustMax[i] * ux[i];
		tauY = tauY + _thrustMax[i] * uy[i];
		tauN = tauN
				+ (-ux[i] * _yPosition[i] + uy[i] * _xPosition[i])
						* _thrustMax[i];
	}

	Function h;
	uint nCosts = getNThrusters() * 3 + 2 * DOF;
	DMatrix S(nCosts, nCosts);
	S.setIdentity();
	DVector ref(nCosts);
	ref.setZero();
	uint costNr = -1;
	for (uint i = 0; i < getNThrusters(); i++) {
		h << ux[i];
		h << uy[i];
		costNr++;
		S(costNr, costNr) = getKu2p(i, directionList[i]);
		costNr++;
		S(costNr, costNr) = getKu2p(i, directionList[i]);
/*		h << ux[i]*cos(alphaOld[i])+uy[i]*sin(alphaOld[i]);
		costNr++;
		S(costNr, costNr) = _dfCost * getKu2p(i, directionList[i]);
		ref(costNr) = uxOld[i]*cos(alphaOld[i])+uyOld[i]*sin(alphaOld[i]);
		
		
		h << uy[i]*cos(alphaOld[i])-ux[i]*sin(alphaOld[i]);
		costNr++;
		ref(costNr) = uyOld[i]*cos(alphaOld[i])-uxOld[i]*sin(alphaOld[i]);
		S(costNr, costNr) = _dAngleCost * getKu2p(i, directionList[i])/(powf(uxOld[i],2)+powf(uyOld[i],2));
		*/
		
	}

	for (uint i = 0; i < DOF; i++) {
		costNr++;
		h << sTauP[i];
		S(costNr, costNr) = 100*getKu2p(0, 1);
		ref(costNr) = -1;

		costNr++;
		h << sTauN[i];
		S(costNr, costNr) = 100*getKu2p(0, 1);
		ref(costNr) = -1;
	}

	for (uint i = 0; i < getNThrusters(); i++) {
		h << sdf[i];
		costNr++;
		S(costNr, costNr) = 100*getKu2p(0, 1);
		ref(costNr) = -1;
	}

	ASSERT(costNr + 1 == nCosts,
			"Internal error in thruster allocation, cost vectors does not have correct length");
	NLP nlp;
	nlp.minimizeLSQ(S, h, ref);
	if (_singularityCostGain > 0 || fixThrusterAngles) {
		Function x;
		CFunction cfun;
		IntermediateState in(getNThrusters() * 2);
		for (unsigned i = 0; i < getNThrusters(); i++) {
			in(i * 2) = ux[i];
			in(i * 2 + 1) = uy[i];
		}
		x << singularityCostFun(in);
		nlp.minimizeLSQ(x);
	}

// Require that the thrust should be equal to desired
	nlp.subjectTo(tau_d[0] == tauX + sTauP[0] - sTauN[0]);
	nlp.subjectTo(tau_d[1] == tauY + sTauP[1] - sTauN[1]);
	nlp.subjectTo(tau_d[2] == tauN + sTauP[2] - sTauN[2]);

	for (uint i = 0; i < getNThrusters(); i++){
		nlp.subjectTo(0 <= sdf[i]);
	}

// Require that the slack variables should be positive
	for (uint i = 0; i < DOF; i++) {
		nlp.subjectTo(0 <= sTauP[i]);
		nlp.subjectTo(0 <= sTauN[i]);
	}

// Require that the power should be within power limits:
//  0 <= f <= 1
// pmax >= Ku2p |f|^1.5 -> |f| <= (pmax/Ku2p)^2/3
// and d u/dt constraint
	vector<Expression> pBus(_nbus,Expression(0.));
	vector<double> minPBus(_nbus,0.);

	for (uint i = 0; i < getNThrusters(); i++) {
        if (_thrustersOn[i] && powerAvailableBus[thruster2bus(i)] > 1E-6
            && powerAvailableThruster[i] > 1E-6) {
		double maxThrust2, minThrust2;
		if (directionList[i] > 0) {
			if ((fOld[i] + _dfMax[i] * _sampleTime < 0) && _thrustersOn[i]) {
				return returnValue("Invalid direction list", LVL_ERROR);
			} else {
				maxThrust2 = min(
						min(1.,
								pow(
										powerAvailableThruster[i]
												/ getKu2p(i, directionList[i]),
										4. / 3.)),
						pow(fOld[i] + _dfMax[i] * _sampleTime, 2));
				minThrust2 = pow(
						max(max(0., _fMin[i]),
								fOld[i] - _dfMax[i] * _sampleTime), 2);
			}
		} else {
			if (fOld[i] - _dfMax[i] * _sampleTime > 0) {
				return returnValue("Invalid direction list", LVL_ERROR);
			} else {
				maxThrust2 = min(
						min(pow(_fMin[i], 2),
								pow(
										powerAvailableThruster[i]
												/ getKu2p(i, directionList[i]),
										4. / 3.)),
						pow(fOld[i] - _dfMax[i] * _sampleTime, 2));
				minThrust2 = pow(min(0., fOld[i] + _dfMax[i] * _sampleTime), 2);
			}
		}
		minThrust2 = min(minThrust2,maxThrust2);

		minPBus[thruster2bus(i)] += pow(minThrust2,.75)*getKu2p(i,directionList[i]);
		pBus[thruster2bus(i)] = pBus[thruster2bus(i)]
				+ (pow((pow(ux[i], 2) + pow(uy[i], 2) + 1e-10), .75)
						- pow(1e-10, .75)) * getKu2p(i, directionList[i]);



		nlp.subjectTo(minThrust2 <= pow(ux[i], 2) + pow(uy[i], 2) + sdf[i]);
		nlp.subjectTo(pow(ux[i], 2) + pow(uy[i], 2) <= maxThrust2);

		//d alpha/dt constraint and alpha
		if (_rotable[i] && !fixThrusterAngles) {
			double dA = min(_dAlphaMax[i] * _sampleTime, PI / 2);
			double alphaMin = alphaOld[i] - dA;
			double alphaMax = alphaOld[i] + dA;
			if (directionList[i] > 0) {
				nlp.subjectTo(
						0 <= -sin(alphaMin) * ux[i] + cos(alphaMin) * uy[i]);
				nlp.subjectTo(
						0 <= +sin(alphaMax) * ux[i] - cos(alphaMax) * uy[i]);
			} else {
				nlp.subjectTo(
						0 >= -sin(alphaMin) * ux[i] + cos(alphaMin) * uy[i]);
				nlp.subjectTo(
						0 >= +sin(alphaMax) * ux[i] - cos(alphaMax) * uy[i]);
			}
		} else {
			// Constraint the thrust to be on the line
			nlp.subjectTo(
					0 == -sin(alphaOld[i]) * ux[i] + cos(alphaOld[i]) * uy[i]);
			// Constraint the thrust to be in correct direction (sign)

			if (directionList[i] > 0) {
				nlp.subjectTo(
						0
								<= cos(alphaOld[i]) * ux[i]
										+ sin(alphaOld[i]) * uy[i]);
			} else {
				nlp.subjectTo(
						0
								>= cos(alphaOld[i]) * ux[i]
										+ sin(alphaOld[i]) * uy[i]);
			}


		}
        } else {
            // If disconnected or low power
            nlp.subjectTo(0 == ux[i]);
			nlp.subjectTo(0 == uy[i]);
        }
	}

	for (uint i = 0; i< getNThrusters(); i++){
		unsigned int bus = thruster2bus(i);
		if (minPBus[bus] < powerAvailableBus[bus])
			nlp.subjectTo(sdf[i] == 0);
	}

	for (uint i = 0; i < _nbus; i++) {
		nlp.subjectTo(pBus[i] <= powerAvailableBus[i]);

	}

	OptimizationAlgorithm algorithm(nlp);
	algorithm.set(PRINT_COPYRIGHT, BT_FALSE);
	switch (_printLevel){
	case PL_HIGH:
		cout << "Solving thruster demand for with direction list:" << endl;
		printDirectionList(directionList);
		algorithm.set(PRINTLEVEL, MEDIUM);
		break;
	case PL_MEDIUM:
		cout << "Solving thruster demand for with direction list:" << endl;
		printDirectionList(directionList);
		algorithm.set(PRINTLEVEL, LOW);
		break;
	case PL_LOW:
		cout << "Solving thruster demand for with direction list:" << endl;
		printDirectionList(directionList);
	case PL_NONE:
		algorithm.set(PRINTLEVEL,  NONE);
		break;
	}



//	algorithm.set(PRINTLEVEL, LOW);
//algorithm.set(MAX_NUM_ITERATIONS, 10);
//	algorithm.set(HESSIAN_APPROXIMATION, GAUSS_NEWTON);
	algorithm.set(KKT_TOLERANCE, 1e-6);

	if (!_pLast.isZero()) {
		VariablesGrid pInt = VariablesGrid(getNThrusters() * 3 + 2 * DOF, 1);
		pInt.setTime(0.);
		pInt.setVector(0, _pLast);
		algorithm.initializeParameters(pInt);
	}

	returnValue ret = algorithm.solve();
	if(ret == SUCCESSFUL_RETURN){
		algorithm.getParameters(pResult);
		for (uint i = 0; i < 2 * getNThrusters(); i += 2) {
			double ux = pResult(i);
			double uy = pResult(i + 1);
			fOut[i / 2] = sqrt(pow(ux, 2) + pow(uy, 2));
			if (fOut[i / 2] < 1e-4)
				alphaOut[i / 2] = alphaOld[i / 2];
			else
				alphaOut[i / 2] = atan2(uy, ux);

		}

		cost = algorithm.getObjectiveValue();
        if (cost == 0)
            throw new WrongAcadoException; //"You are using a version of acado which always gives a cost of zero. Check github.com/tibnor/acado");
		fixDirection(fOut, alphaOut, alphaOld);
	}

	ux[0].clearStaticCounters();
	return ret;
}

/// Initialize optimization (for a warmer start)
void ThrustAllocation::initialize(const double f[], const double alpha[]) {
	_pLast.setZero();
	vector<double> ux(getNThrusters());
	vector<double> uy(getNThrusters());
	convertFtoU(f, alpha, &ux[0], &uy[0]);
	for (unsigned int i = 0; i < getNThrusters(); i++) {
		_pLast(i * 2) = ux[i];
		_pLast(i * 2 + 1) = uy[i];
	}
}

void ThrustAllocation::fixDirection(double f[], double alpha[],
		const double alpha0[]) const {
	for (uint i = 0; i < getNThrusters(); i++) {
		if (cos(alpha[i] - alpha0[i]) < 0) {
			alpha[i] += PI;
			if (alpha[i] > 2 * PI)
				alpha[i] -= 2 * PI;
			f[i] *= -1;
		}
	}

}

vector<signed char> ThrustAllocation::findDirection0(const double* f0) const {
	vector<signed char> directionList(getNThrusters());
	for (uint i = 0; i < getNThrusters(); i++)
		directionList[i] = (f0[i] >= 0) * 2 - 1;

	return directionList;
}

bool ThrustAllocation::isFeasible(const double u[],
		const double powerAvailableBus[], const double powerAvailableThruster[],
		const double uOld[]) const {
    // Sum all power consumption per bus
	vector<double> powerBus(_nbus, 0.);
	for (uint i = 0; i < getNThrusters(); i++) {
		signed char direction = 2 * (uOld[i] >= 0) - 1;
		double power = getKu2p(i, direction) * pow(fabs(u[i]), 1.5);
		if (power > powerAvailableThruster[i])
			return false;
		powerBus[thruster2bus(i)] += power;
	}
	// Check that power consumption of bus is less than or equal to power available of bus
	for (uint i = 0; i < _nbus; i++) {
		if (powerBus[i] > powerAvailableBus[i])
			return false;
	}

	// Check if thrust demand:
	//  - is less than maximum allowed thrust.
	//  - is more than minimum allowed thrust.
	//  - rate of change is less than allowed rate of change
	for (uint i = 0; i < getNThrusters(); i++) {
		if (u[i] > FMAX || u[i] < _fMin[i] || fabs(u[i] - uOld[i]) > _dfMax[i])
			return false;
	}

	return true;
}

uint ThrustAllocation::thruster2bus(const unsigned &i) const {
	return _switchboard2bus[_thruster2switchboard[i]];
}

void ThrustAllocation::scalePower(const uint size, double t[]) {
	for (uint i = 0; i < size; i++) {
		t[i] *= 1 / POWER_BASE;
	}
}

void ThrustAllocation::unScalePower(const unsigned int n, double power[]) {
	for (unsigned int i = 0; i < n; i++)
		power[i] *= POWER_BASE;
}

void ThrustAllocation::scaleThrust(const uint size, double t[]) {
	for (uint i = 0; i < size; i++) {
		t[i] *= 1 / TAU_BASE;
	}
}

void ThrustAllocation::scaleTau(double t[]) {
	t[0] *= 1 / TAU_BASE;
	t[1] *= 1 / TAU_BASE;
	t[2] *= 1 / TAU_BASE / LENGTH_BASE;
}

void ThrustAllocation::scaleLength(const uint size, double t[]) {
	for (uint i = 0; i < size; i++) {
		t[i] *= 1 / LENGTH_BASE;
	}
}

void ThrustAllocation::scaleKt2p(const uint size, double t[]) {
	for (uint i = 0; i < size; i++) {
		t[i] *= pow(TAU_BASE, 1.5) / POWER_BASE;
	}
}

void ThrustAllocation::setPlantConfiguration(const uint switchboard2bus[],
		const uint nbus) {

	_nbus = nbus;
	_switchboard2bus.assign(switchboard2bus, switchboard2bus + _nswitchboards);
	return;
}

void ThrustAllocation::setThrustersOn(
		const double powerAvailableBus[],
		const bool thrustersOnUnchecked[]) {
    for (uint i = 0; i<getNThrusters(); i++) {
        _thrustersOn[i] = thrustersOnUnchecked[i];
    }
	for (uint i = 0; i < _nbus; i++) {
		// Check if power available is low
		if (powerAvailableBus[i] < .0001) {
			// If low turn of all connected thrusters
			for (uint j = 0; j < getNThrusters(); j++) {
				if (_thrustersOn[j] && i == thruster2bus(j))
				  _thrustersOn[j] = false;
			}
		}

	}
}

bool ThrustAllocation::isAnyThrusterConnected() const {
	for (uint i = 0; i < getNThrusters(); i++)
		if (_thrustersOn[i])
			return true;

	return false;
}

template<class T> void printValue(string label,T val){
  cout << label << ": " << val << endl;
}

template<class T> void printValue(string label,T val, uint length){
  cout << label << ": ";
  for (uint i = 0; i<length; i++)
    cout << val[i] << ", ";
  cout << endl;
}


template<class T> void printValue(string label,vector<T> val){
  printValue(label,val,val.size());
}


void ThrustAllocation::dump() const{
    printValue("_nthrusters",_nthrusters);
    printValue("_nswitchboards",_nswitchboards);
    printValue("_nbus",_nbus);
    printValue("_thrustersOn",_thrustersOn,_nthrusters); ///< Thrusters connected and with power
    printValue("_xPosition",_xPosition); ///< SCALED x-position of thrusters
    printValue("_yPosition",_yPosition); ///< SCALED y-position of thrusters
    printValue("_dfMax",_dfMax); ///< Maximum change of thrust demand [%/iteration]
    printValue("_dAlphaMax",_dAlphaMax); ///< Maximum change of azimuth angle [rad/iteration]
    printValue("_fMin",_fMin); ///< Minimum thrust demand (maximum is set to 1)
    printValue("_Ku2p",_Ku2p);   ///< SCALED Thruster input to power P = Ku2p*u^1.5
    printValue("_thrustMax",_thrustMax);  ///< SCALED maximum thrust of thrusters
    printValue("_rotable",_rotable); ///< True if thruster is rotable
    printValue("_thruster2switchboard",_thruster2switchboard);
    printValue("_reverseThrustPowerGain",_reverseThrustPowerGain); ///< Extra power used when thrust is reversed
    printValue("_switchboard2bus",_switchboard2bus); ///< Index is switchboard index, value is bus index
    printValue("_sampleTime",_sampleTime);
    _pLast.print("_pLast");
    printValue("_singularityCostGain",_singularityCostGain);
    printValue("_singularityCostSmoother",_singularityCostSmoother);
    printValue("_printLevel",_printLevel);
}
