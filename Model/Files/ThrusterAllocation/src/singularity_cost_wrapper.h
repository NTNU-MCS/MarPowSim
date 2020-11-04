/*
 * Wrapper for numerical calculation of singularity avoidance cost from c function to ACADO::CFunction
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	Apr 28, 2014	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef SINGULARITY_COST_WRAPPER_H_
#define SINGULARITY_COST_WRAPPER_H_

#include <acado/function/c_function.hpp>

/**\brief  A class for wrapping the C++-functions of singularity avoidance cost
 *
 * This function is used to generate ACADO::CFunction for singularity avoidance cost.
 * The value can be CFunction contains both a value function and a gradiant/forwardAD of the singularity avoidance cost.
 * The cost function is defined as \f[\frac{\varrho}{\epsilon+|T(\alpha)|}\f]
 */
class SingularityCostWrapper {
public:

	SingularityCostWrapper(const unsigned int nThrusters, ///< Number of thrusters
			const double epsilon, ///< Singularity avoidance cost smoothing
			const double varrho ///< Singularity avoidance cost gain
			);
	~SingularityCostWrapper();
	/** Constructs a ACADO CFunction with singularity avoidance cost (and gradiant of it)
	* Can be used in cost function
	* @param isOn boolean vector of thrusters in operation.
	*/
	ACADO::CFunction getCFunction(const bool* isOn
			);
private:
	/**
	 * \brief A struct for storing parameters for CFunction object
	 */
	struct userData {
		const double epsilon;///< Singularity avoidance cost smoothing
		const double varrho;///< Singularity avoidance cost gain
		const bool* isOn;
		const unsigned int nThrusters;
        userData(const double argepsilon, const double argvarrho, const bool * const argisOn, const unsigned int argnThrusters) : epsilon(argepsilon), varrho(argvarrho), 
                isOn(argisOn), nThrusters(argnThrusters) {}
	};
	userData _userData;
	static void getSingularityCost(double *x, double *f, void *userData);
	static void forwardAD(int number, double *x, double *seed, double *f, double *df, void *userData);
};

#endif /* SINGULARITY_COST_WRAPPER_H_ */
