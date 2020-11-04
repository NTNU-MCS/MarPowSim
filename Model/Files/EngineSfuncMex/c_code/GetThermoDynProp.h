/*
 * File: GetThermoDynProp.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETTHERMODYNPROP_H__
#define __GETTHERMODYNPROP_H__

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

  extern void GetThermoDynProp(double P, double T, const double x[12], double *R,
    double *Cp, double *Cv, double *u, double *ht, double *s, double *rho);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetThermoDynProp.h
 *
 * [EOF]
 */
