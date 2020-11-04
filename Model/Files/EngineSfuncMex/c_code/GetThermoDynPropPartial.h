/*
 * File: GetThermoDynPropPartial.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETTHERMODYNPROPPARTIAL_H__
#define __GETTHERMODYNPROPPARTIAL_H__

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

  extern void GetThermoDynPropPartial(double P, double T, const double x[12],
    const double dx[36], double *R, double *h, double *s, double *u, double *RP,
    double *RT, double *RF, double *uP, double *uT, double *uF, double *sP,
    double *sT, double *sF, double *Cp, double *Cv);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetThermoDynPropPartial.h
 *
 * [EOF]
 */
