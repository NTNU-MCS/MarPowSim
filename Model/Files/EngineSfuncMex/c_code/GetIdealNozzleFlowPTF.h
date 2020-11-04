/*
 * File: GetIdealNozzleFlowPTF.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

#ifndef __GETIDEALNOZZLEFLOWPTF_H__
#define __GETIDEALNOZZLEFLOWPTF_H__

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

  extern void GetIdealNozzleFlowPTF(double Cd, double A, double p_in, double
    p_out, double T_in, double F_in, double fs, double *m_dot, double *H_dot,
    double *m_b_dot);

#ifdef __cplusplus

}
#endif
#endif

/*
 * File trailer for GetIdealNozzleFlowPTF.h
 *
 * [EOF]
 */
