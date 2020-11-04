/*
 * File: GetIdealNozzleFlow.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETIDEALNOZZLEFLOW_H__
#define __GETIDEALNOZZLEFLOW_H__

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

  extern void GetIdealNozzleFlow(double Cd, double A, double p_in, double p_out,
    double T_in, const double X_in[12], double *m_dot, double *h_dot, double
    N_dot[12]);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetIdealNozzleFlow.h
 *
 * [EOF]
 */
