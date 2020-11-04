/*
 * File: GetMEMbZach.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 14-Sep-2015 17:04:06
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "GetAirDensity.h"
#include "GetAirThermalConduct.h"
#include "GetAirViscosity.h"
#include "GetCompCombGas.h"
#include "GetCorrAirComp.h"
#include "GetDensityHCVapor.h"
#include "GetDiffusivityHCVaporToAir.h"
#include "GetEquilGrill.h"
#include "GetEquilOlikara.h"
#include "GetFuelPropertyN_Dodecane.h"
#include "GetHTCoeffHTX.h"
#include "GetIdealNozzleFlow.h"
#include "GetIdealNozzleFlowPTF.h"
#include "GetMEMbZach.h"
#include "GetPTF.h"
#include "GetPTFV1.h"
#include "GetPTx.h"
#include "GetTFromPhF.h"
#include "GetT_atm_p.h"
#include "GetThdynCombGasZach.h"
#include "GetThdynCombGasZachV1.h"
#include "GetThermalPropertyHCVaporReal.h"
#include "GetThermoDynProp.h"
#include "GetThermoDynPropPartial.h"
#include "GetTotalStaticPT.h"
#include "GetViscosityHCVapor.h"

/* Function Definitions */

/*
 * Calculates the mass, internal energy and the burned fuel mass in the
 * control volume with given p, T, F, V, fs by the thermodynamic property
 * calculation using Zacharia's method and ideal gas law.
 *    input : p[Pa], T[K], F, V[m3], fs
 *    output : m[kg], E[J], mb[kg]
 *
 *  Created by Kevin Koosup Yum, NTNU, 15/04/2015
 * Arguments    : double p
 *                double T
 *                double F
 *                double V
 *                double fs
 *                double *m
 *                double *E
 *                double *mb
 * Return Type  : void
 */
void GetMEMbZach(double p, double T, double F, double V, double fs, double *m,
                 double *E, double *mb)
{
  double TT;
  double b_V;
  double FC2;
  double FC3;
  double FC4;
  double FC5;
  double FC6;
  double FC7;
  double FC11;
  double R;

  /* The routine calculates the thermodynamic property of the combustion gas */
  /* given pressure, temperature and the fuel air equivalent ratio using */
  /* Zacharias' method */
  /*  */
  /*  Input */
  /*    P : Pressure (Pa) */
  /*    T : Temperature (K) */
  /*    F : Fuel-air equivalent ratio */
  /*    fs : Stoichiometric fuel-air ratio */
  /*  Output */
  /*    R : Gas constant */
  /*    h :	Specific enthalpy */
  /*    s : Specific Entropy */
  /*    u : Specific internal energy */
  /*    RF : Partial derivative of R w.r.t. F */
  /*    RP : Partial derivative of R w.r.t. P */
  /*    RT : Partial derivative of R w.r.t. T */
  /*    uF : Partial derivative of u w.r.t. F */
  /*    uP : Partial derivative of u w.r.t. P */
  /*    uT : Partial derivative of u w.r.t. T */
  /*    sF : Partial derivative of s w.r.t. F */
  /*    sP : Partial derivative of s w.r.t. P */
  /*    sT : Partial derivative of s w.r.t. T */
  /*    CP : Specific heat at constant pressure */
  /*    CV : Specific heat at constant volume */
  /*    K  : Ratio of specific heats */
  /*  FC0 = 1.01972e-5; */
  TT = 0.001 * T;
  b_V = (1.0 - F) / (1.0 + fs * F);
  FC2 = 0.000277105 - 9.00711E-5 * b_V;
  FC3 = 0.000642217 - 9.8367E-5 * b_V;
  FC4 = 0.008868 - 0.006131 * b_V;
  FC5 = TT * TT;
  FC6 = exp(FC4 / FC5) / TT;
  FC7 = 1.01972E-5 * p / TT;

  /*  Verified */
  FC11 = 8314.7 * (F / 3.0 + 4.773384) / (9.344254 * F + 138.2266);
  R = FC11 * (FC7 * (FC3 - FC2 * FC6) + 1.0);
  *m = p * V / (R * T);
  *E = (FC11 * T * (((3.514956 - 0.005026 * b_V) + TT * ((0.131438 - 0.383504 *
            b_V) + TT * ((0.477182 + 0.185214 * b_V) + TT * ((-0.287367 -
              0.0694862 * b_V) + TT * ((0.0742561 + 0.0164041 * b_V) + TT *
              ((-0.00916344 - 0.00204537 * b_V) + TT * (0.000439896 + 0.00010161
    * b_V))))))) - FC7 * (FC2 * FC6 * ((1.0 + FC4 / FC5) + 1.0) - FC3) / TT * TT)
        - R * T) * *m;
  *mb = *m * F * fs / (1.0 + F * fs);
}

/*
 * File trailer for GetMEMbZach.c
 *
 * [EOF]
 */
