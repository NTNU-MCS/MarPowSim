/*
 * File: GetCompCombGas.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETCOMPCOMBGAS_H__
#define __GETCOMPCOMBGAS_H__

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

  extern void GetCompCombGas(double P, double T, double F, double T_L, double
    T_H, const double Fc[4], const double x_air[12], double x[12]);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetCompCombGas.h
 *
 * [EOF]
 */
