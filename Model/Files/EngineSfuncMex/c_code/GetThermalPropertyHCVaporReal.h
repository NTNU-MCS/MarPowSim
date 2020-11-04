/*
 * File: GetThermalPropertyHCVaporReal.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETTHERMALPROPERTYHCVAPORREAL_H__
#define __GETTHERMALPROPERTYHCVAPORREAL_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "ThdynPack_types.h"

/* Function Declarations */
#ifdef __cplusplus

extern "C" {

#endif

  extern void GetThermalPropertyHCVaporReal(double p, double T, double MW,
    double T_CR, double p_CR, double W, double *rho_v, double *Cp_v, double
    *Cv_v, double *lambda);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetThermalPropertyHCVaporReal.h
 *
 * [EOF]
 */
