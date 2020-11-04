

/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void GetXCombGas_Outputs_wrapper(const real_T *p,
			const real_T *T,
			const real_T *F,
			real_T *x,
			const real_T  *fs, const int_T  p_width0,
			const real_T  *T_L, const int_T  p_width1,
			const real_T  *T_H, const int_T  p_width2,
			const real_T  *FC, const int_T  p_width3,
			const real_T  *xAir, const int_T  p_width4)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
GetCompCombGas(p[0],T[0],F[0],T_L[0],T_H[0],FC,xAir,x);
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
