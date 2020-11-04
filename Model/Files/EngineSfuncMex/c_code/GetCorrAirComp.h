/*
 * File: GetCorrAirComp.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETCORRAIRCOMP_H__
#define __GETCORRAIRCOMP_H__

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

  extern void GetCorrAirComp(double T_amb, double p_amb, double humid, double
    Mf_a, double Mf_a_corr[5], double Wf_a_corr[5], double *MW, double *H_a);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetCorrAirComp.h
 *
 * [EOF]
 */
