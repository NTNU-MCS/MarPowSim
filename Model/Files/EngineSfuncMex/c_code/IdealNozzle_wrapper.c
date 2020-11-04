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
  *   Created: Tue Jun 17 16:51:43 2014
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
void IdealNozzle_Outputs_wrapper(const real_T *A_eff,
                          const real_T *pu,
                          const real_T *Tu,
                          const real_T *Fu,
                          const real_T *pd,
                          const real_T *Td,
                          const real_T *Fd,
                          real_T *dmu,
                          real_T *deu,
                          real_T *dmbu,
                          real_T *dmd,
                          real_T *ded,
                          real_T *dmbd, 
                           const real_T  *fs, const int_T p_width0)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
//parameters
//  double fs;      //Stoichiometric fuel-air ratio
//variables
double aa, a, d;   //Pressure ratio up/down and in/out, flow direction
double p_AV, T_AV, F_AV; //Inlet pressure, temperature, FA equivalent ratio
double hu,su,uu,Ru,RFu,Rpu,RTu,uFu,upu,uTu,Cpu,Cvu,Ku; 
                    //Inlet thermodynamic properties
double pc, c;          //pressure ratio for sonic flow
double ksim, ksin, ksil, ksi, ksi1;
//code
// First determine the flow direction
aa = *pu / *pd;
if (aa > 1) {
    p_AV	=pp_in.e; T_AV=pT_in.e; F_AV= pF_in.e;	d	= 1; 	a	=aa;
}
else {
    p_AV	=pp_out.e; T_AV=pT_out.e; F_AV= pF_out.e;	d	=-1; a	=1.0/aa;
}

aa = *pu / *pd;
T_mean = 0.5*(*Tu + *Td);
if (aa > 1) {
    p_in = *pu;      T_in = *Tu; 
    F_in= *Fu;	    p_out = *pd;
    a = aa;
}
else {
	p_in = *pd;     T_in = *Td;
    F_in = *Fd;    a = 1.0 / aa;
	p_out = *pu;
}

GetIdealNozzleFlow(1, *A_eff, p_in, p_out, T_in, F_in, *fs, 
        dmu, deu, dmbu);
*dmu = *dmu*d;
*deu = *deu*d;
*dmbu = *dmbu*d;
*dmd = *dmu;
*ded = *deu;
*dmbd = *dmbu;
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}