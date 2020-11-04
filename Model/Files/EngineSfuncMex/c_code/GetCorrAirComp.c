/*
 * File: GetCorrAirComp.c
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
#include "ThdynPack_rtwutil.h"

/* Function Definitions */

/*
 * Calculate the corrected mole and mass fraction of molecules (Mf_a, Wf_a)
 * in air with humidity, water content(w_H2O, g/kg) and molecular weight(MW)
 * of air at given temperature in Kelvin and relative humidity in %.
 * Vapor pressure of water at saturated condition(bar) is used from
 * ISO 8178-1 Eqn (A.14), Wexler and Greenspan equation
 *
 *  input
 *    T_amb : Ambient temperature [K]
 *    p_amb : Ambient pressure [Pa]
 *    humid : Relative humidity [%]
 *    Mf_a : Dry air composition mole ratio to oxygen  (O2;N2;Ar;CO2)
 *             e.g. [1;3.7274;0.0444;0.0014]
 *  Output
 *    Mf_a_corr : Air composition mole ratio to oxigen (O2:N2:Ar:CO2:H2O)
 *    Wf_a_corr : Air composition weight ratio to oxigen (O2:N2:Ar:CO2:H2O)
 *    MW : Molecular weight
 *    H_a : water content (g H2O / kg Air)
 *
 *  Created by Kevin Yum
 * Arguments    : double T_amb
 *                double p_amb
 *                double humid
 *                double Mf_a
 *                double Mf_a_corr[5]
 *                double Wf_a_corr[5]
 *                double *MW
 *                double *H_a
 * Return Type  : void
 */
void GetCorrAirComp(double T_amb, double p_amb, double humid, double Mf_a,
                    double Mf_a_corr[5], double Wf_a_corr[5], double *MW, double
                    *H_a)
{
  double M_f_a;
  double p_p_H2O;
  int k;
  double M_f_a_corr[5];
  double y;
  double b_y;
  double c_y;
  double d_y[5];
  static const double b[5] = { 31.9988, 28.011, 39.9481, 44.01, 18.01534 };

  double b_MW[5];
  int i2;
  static const double b_b[25] = { 31.9988, 0.0, 0.0, 0.0, 0.0, 0.0, 28.011, 0.0,
    0.0, 0.0, 0.0, 0.0, 39.9481, 0.0, 0.0, 0.0, 0.0, 0.0, 44.01, 0.0, 0.0, 0.0,
    0.0, 0.0, 18.01534 };

  M_f_a = Mf_a / Mf_a;

  /* Vapor pressure of water at saturated condition(bar)  */
  /* ISO 8178-1 Eqn (A.14), Wexler and Greenspan equation */
  /* Partial pressure of water                        */
  p_p_H2O = humid / 100.0 * exp((((((((((-12.150799 * log(T_amb) - 8499.22 *
    rt_powd_snf(T_amb, -2.0)) - 7423.1865 * (1.0 / T_amb)) + 96.1635147) +
    0.024917646 * T_amb) - 1.3160119E-5 * (T_amb * T_amb)) - 1.1460454E-8 *
    rt_powd_snf(T_amb, 3.0)) + 2.1701289E-11 * rt_powd_snf(T_amb, 4.0)) -
    3.610258E-15 * rt_powd_snf(T_amb, 5.0)) + 3.8504519E-18 * rt_powd_snf(T_amb,
    6.0)) - 1.4317E-21 * rt_powd_snf(T_amb, 7.0));

  /* no. of mole of water for 1 mole of oxygen in the air */
  /* Corrected mole & wt fraction including water content */
  for (k = 0; k < 4; k++) {
    M_f_a_corr[k] = M_f_a;
  }

  M_f_a_corr[4] = M_f_a * p_p_H2O / (p_amb * 100.0 - p_p_H2O);
  M_f_a = M_f_a_corr[0];
  p_p_H2O = M_f_a_corr[0];
  y = M_f_a_corr[0];
  b_y = M_f_a_corr[0];
  c_y = M_f_a_corr[0];
  for (k = 0; k < 4; k++) {
    M_f_a += M_f_a_corr[k + 1];
    p_p_H2O += M_f_a_corr[k + 1];
    y += M_f_a_corr[k + 1];
    b_y += M_f_a_corr[k + 1];
    c_y += M_f_a_corr[k + 1];
  }

  d_y[0] = M_f_a;
  d_y[1] = p_p_H2O;
  d_y[2] = y;
  d_y[3] = b_y;
  d_y[4] = c_y;
  *MW = 0.0;
  for (k = 0; k < 5; k++) {
    M_f_a = M_f_a_corr[k] / d_y[k];
    *MW += M_f_a * b[k];
    Mf_a_corr[k] = M_f_a;
  }

  b_MW[0] = *MW;
  b_MW[1] = *MW;
  b_MW[2] = *MW;
  b_MW[3] = *MW;
  b_MW[4] = *MW;
  for (k = 0; k < 5; k++) {
    M_f_a_corr[k] = 0.0;
    for (i2 = 0; i2 < 5; i2++) {
      M_f_a_corr[k] += Mf_a_corr[i2] * b_b[i2 + 5 * k];
    }

    Wf_a_corr[k] = M_f_a_corr[k] / b_MW[k];
  }

  /*  Humidity of the intake air (g water per kg dry air) */
  *H_a = Mf_a_corr[4] * 18.01534 / (*MW - Mf_a_corr[4] * 18.01534) * 1000.0;
}

/*
 * File trailer for GetCorrAirComp.c
 *
 * [EOF]
 */
