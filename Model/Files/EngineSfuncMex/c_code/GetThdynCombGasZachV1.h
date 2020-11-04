/*
 * File: GetThdynCombGasZachV1.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETTHDYNCOMBGASZACHV1_H__
#define __GETTHDYNCOMBGASZACHV1_H__

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

  extern void GetThdynCombGasZachV1(double P, double T, double F, double fs,
    double *R, double *h, double *s, double *u, double *RF, double *RP, double
    *RT, double *uF, double *uP, double *uT, double *sF, double *sP, double *sT,
    double *Cp, double *Cv, double *K);
  extern void b_GetThdynCombGasZachV1(double P, double T, double F, double fs,
    double *R, double *h, double *s, double *u, double *RF, double *RP, double
    *RT, double *uF, double *uP, double *uT, double *sF, double *sP, double *sT);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetThdynCombGasZachV1.h
 *
 * [EOF]
 */
