

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
void SFCCyc_Outputs_wrapper(const real_T *mqfCyc,
			const real_T *wCyc,
			const real_T *resetCyc,
			real_T *SFC,
            real_T *mqf)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
	if (wCyc[0] < 1){
		SFC[0] = 0;
	}
	else {
		SFC[0] = mqf[0] / (wCyc[0] + 1e-3) * 3.6e9;
	}


/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

void SFCCyc_Update_wrapper(const real_T *mqfCyc,
			const real_T *resetCyc,
            real_T *mqf)
{
    if (resetCyc[0] == 1){
        mqf[0] = mqfCyc[0];
    }
}