/*
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	Apr 28, 2014	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "singularity_cost_wrapper.h"

void singularityCost(double epsilon, double varrho, const bool isOn[],
		const double* x, double* f);
void singularityCostGradient(double epsilon, double varrho, const bool isOn[],
		const double x[], double f[12]);

SingularityCostWrapper::SingularityCostWrapper(const unsigned int nThrusters, ///< Number of thrusters
		const double epsilon, const double varrho) :
		_userData( epsilon, varrho, 0, nThrusters )
        {

}

SingularityCostWrapper::~SingularityCostWrapper() {

}

ACADO::CFunction SingularityCostWrapper::getCFunction(const bool* isOn) {
	_userData.isOn = isOn;
	ACADO::CFunction cfun(1, getSingularityCost, forwardAD, 0);
	cfun.setUserData((void *) &_userData);
	return cfun;
}

void SingularityCostWrapper::getSingularityCost(double *x, double *f,
		void *userDataPtr) {
	userData* data = (userData*) userDataPtr;
	singularityCost(data->epsilon, data->varrho, data->isOn, x, f);
}

void SingularityCostWrapper::forwardAD(int number, double *x, double *seed,
		double *f, double *df, void *userDataPtr) {
	userData* data = (userData*) userDataPtr;
	singularityCost(data->epsilon, data->varrho, data->isOn, x, f);
	double* gradient = new double[data->nThrusters * 2];
	singularityCostGradient(data->epsilon, data->varrho, data->isOn, x,
			gradient);
	*df = 0;
	for (unsigned i = 0; i < data->nThrusters * 2; i++)
		*df += seed[i] * gradient[i];
	delete[] gradient;
}
