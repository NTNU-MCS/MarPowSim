/*
 * File: GetPTFV1.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETPTFV1_H__
#define __GETPTFV1_H__

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

  extern void GetPTFV1(double m, double m_b, double E, double V, double T_prev,
                       double R_prev, double u_prev, double Cv_prev, double fs,
                       double *p, double *T, double *F);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetPTFV1.h
 *
 * [EOF]
 */
