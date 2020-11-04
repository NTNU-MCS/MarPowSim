/*
  *
  *   --- THIS FILE GENERATED BY S-FUNCTION BUILDER: 3.0 ---
  *
  *   This file is a wrapper S-function produced by the S-Function
  *   Builder which only recognizes certain fields.  Changes made
  *   outside these fields will be lost the next time the block is
  *   used to load, edit, and resave this file. This file will be overwritten
  *   by the S-function Builder block. If you want to edit this file by hand, 
  *   you must change it only in the area defined as:  
  *
  *        %%%-SFUNWIZ_wrapper_XXXXX_Changes_BEGIN 
  *            Your Changes go here
  *        %%%-SFUNWIZ_wrapper_XXXXXX_Changes_END
  *
  *   For better compatibility with the Simulink Coder, the
  *   "wrapper" S-function technique is used.  This is discussed
  *   in the Simulink Coder User's Manual in the Chapter titled,
  *   "Wrapper S-functions".
  *
  *   Created: Wed Jul 23 01:22:32 2014
  */


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
void combState_Outputs_wrapper(const real_T *phi,
                          const real_T *phiInj,
                          const real_T *uGov,
                          const real_T *omega,
                          const real_T *temp,
                          const real_T *p,
                          const real_T *combState,
                          const real_T *phiIg,
                          const real_T *mqf,        
                          real_T *combStateO,
                          real_T *phiComb,
                          real_T *phiIgO,
                          real_T *mqfO ,
                          const real_T  *mqfCycMax, const int_T  p_width0, 
                          const real_T  *wiebePara, const int_T p_width1,
                          const real_T  *nStroke,   const int_T p_width2)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
double phi360, phi360int;
double dphiComb;
double igDel;
const double pi = 3.14159265359;
dphiComb = wiebePara[2] + wiebePara[5];
phi360 = *phi*180/pi;
phi360 = (((int)floor(phi360))%((int)(nStroke[0]*180))) + (phi360 - floor(phi360));
if (phi360 > nStroke[0]*90) {
    *phiComb = phi360 - nStroke[0]*180;
}
else {
    *phiComb = phi360;
}

if ((*phiComb > *phiInj) & (*combState == 0)) {
    *combStateO = 1;
    *mqfO = *mqfCycMax * *uGov;
    igDel = 1e-3*0.0405*exp(5473/(*temp))*pow((*p/100000),-0.757);
    igDel = (*omega*igDel)*180/pi;
    if (igDel > 30) {
        *combStateO = 5;
    }
    *phiIgO = *phiInj + igDel;
}
else {
    if ((*phiComb > *phiIg ) & (*phiComb < *phiIg + dphiComb ) & (*combState == 1)) {
        *combStateO = 2;
        *phiIgO = *phiIg;
        *mqfO = *mqf;
    }
    else {
        if ((*phiComb > *phiIg + dphiComb ) & (*combState == 2)) {
        *combStateO = 5;
        *phiIgO = *phiIg;
        *mqfO = *mqf;        
        }
        else {
            if ((*phiComb > -20)&(*phiComb < -15)) {
                *combStateO = 0;
                *phiIgO = 0;
                *mqfO = *mqf;
            }
            else {
                *combStateO = *combState;
                *phiIgO = *phiIg;
                *mqfO = *mqf;            
            }
        }
    }
}


/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}
