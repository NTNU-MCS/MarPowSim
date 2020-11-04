/*
 * File: GetFuelPropertyN_Dodecane.c
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
 * The function calculates the thermodynamic and transportation property of
 * the liquid n-dodecane fuel
 *    input
 *        p : Pressure of the liquid fuel in Pa
 *        T : Liquid fuel temperature in Kelvin
 *    Output
 *        p_v : vapor pressure of the liquid in Pa
 *        rho_l : density of the fuel in kg/m3
 *        h_l : specific enthalpy of the fuel in J/kg (0 at 0K)
 *        Cp_l : isobaric specific heat capacity in J/kg/K
 *        h_fg : specific latent heat of vaporization in J/kg
 *        mu_l : dynamic viscosity in Ns/m2
 *        lambda_l : Thermal conductivity in W/m/K
 *        sigma_l : Surface tension in N/m
 *
 *    Created by Kevin Koosup Yum (NTNU) on 22 August
 *
 *  Ref
 *    Kouremenos, D. A., C. D. Rakopoulos, et al. (1990). "A FORTRAN program
 *    for calculating the thermodynamic and transport properties of diesel
 *    fuel." Advances in Engineering Software (1978) 12(4): 190-196.
 * Arguments    : double p
 *                double T
 *                double *p_v
 *                double *rho_l
 *                double *h_l
 *                double *Cp_l
 *                double *h_fg
 *                double *mu_l
 *                double *lambda_l
 *                double *sigma_l
 * Return Type  : void
 */
void GetFuelPropertyN_Dodecane(double p, double T, double *p_v, double *rho_l,
  double *h_l, double *Cp_l, double *h_fg, double *mu_l, double *lambda_l,
  double *sigma_l)
{
  double T_R[8];
  int i;
  double p_R[4];
  double dv7[4];
  double d2;
  static const double a[4] = { 14.50170196, -14.9159925, -8.86263462,
    0.414336894 };

  double dv8[5];
  double b_a[4];
  int i5;
  static const double c_a[20] = { 1105.3, -1329.8, 1663.7, -1069.7, -31.164,
    147.71, -246.21, 169.74, 1.4274, -5.4041, 8.6951, -7.685, -0.0052975,
    -0.055594, 0.099981, 0.064605, -0.00046749, 0.0035522, -0.0058678, 0.0014729
  };

  double T_RB;
  double T_RB2;
  double T_RB3;
  double dv9[4];
  static const double d_a[4] = { 3331.5840000000003, -6193.63, 9064.86,
    -3179.248 };

  double dv10[4];
  double dv11[8];
  static const double e_a[8] = { 2.140406765E+7, -2.394932736E+8, 1.154699081E+9,
    -3.051087445E+9, 4.767135466E+9, -4.406307056E+9, 2.232100727E+9,
    -4.784187787E+8 };

  double dv12[6];
  static const double f_a[6] = { 3.21248, -0.0381521, 0.000240018, -8.33717E-7,
    1.4875E-9, -1.05978E-12 };

  /* % Constants for the fuel N-dodecane */
  /* Boiling temperature at 1 atm in K */
  /* Critical temperature in K */
  /* Critical pressure in Pa */
  /* Accentric factor */
  /* % Calculation of reduced temperature and pressure */
  memset(&T_R[0], 0, sizeof(double) << 3);
  T_R[0] = T / 658.1;
  for (i = 0; i < 6; i++) {
    T_R[i + 1] = T_R[i] * T_R[0];
  }

  T_R[7] = log(T_R[0]);
  for (i = 0; i < 4; i++) {
    p_R[i] = 0.0;
  }

  p_R[0] = p / 1.817E+6;
  for (i = 0; i < 3; i++) {
    p_R[i + 1] = p_R[i] * p_R[0];
  }

  /* % Calculation of vapor pressure */
  dv7[0] = 1.0;
  dv7[1] = 1.0 / T_R[0];
  dv7[2] = T_R[7];
  dv7[3] = T_R[5];
  d2 = 0.0;
  for (i = 0; i < 4; i++) {
    d2 += a[i] * dv7[i];
  }

  *p_v = exp(d2) * 1.817E+6;

  /* % Calculation of liquid density */
  dv8[0] = 1.0;
  for (i = 0; i < 4; i++) {
    dv8[i + 1] = p_R[i];
  }

  for (i = 0; i < 4; i++) {
    b_a[i] = 0.0;
    for (i5 = 0; i5 < 5; i5++) {
      b_a[i] += c_a[i + (i5 << 2)] * dv8[i5];
    }
  }

  dv7[0] = 1.0;
  for (i = 0; i < 3; i++) {
    dv7[i + 1] = T_R[i];
  }

  *rho_l = 0.0;
  for (i = 0; i < 4; i++) {
    *rho_l += b_a[i] * dv7[i];
  }

  /* % Liquid specific enthalpy */
  T_RB = T / 489.43;
  T_RB2 = T_RB * T_RB;
  T_RB3 = T_RB2 * T_RB;
  dv9[0] = 1.0;
  dv9[1] = T_RB / 2.0;
  dv9[2] = T_RB2 / 3.0;
  dv9[3] = T_RB3 / 4.0;
  d2 = 0.0;
  for (i = 0; i < 4; i++) {
    d2 += d_a[i] * dv9[i];
  }

  *h_l = d2 * T;

  /* % Liquid specific isobaric heat capacity */
  dv10[0] = 1.0;
  dv10[1] = T_RB;
  dv10[2] = T_RB2;
  dv10[3] = T_RB3;
  *Cp_l = 0.0;
  for (i = 0; i < 4; i++) {
    *Cp_l += d_a[i] * dv10[i];
  }

  /* % Latent heat of vaporization */
  if (T_R[0] <= 0.4) {
    *h_fg = 52138.6 * rt_powd_snf(658.1 - T, 0.38);
  } else {
    dv11[0] = 1.0;
    for (i = 0; i < 7; i++) {
      dv11[i + 1] = T_R[i];
    }

    *h_fg = 0.0;
    for (i = 0; i < 8; i++) {
      *h_fg += e_a[i] * dv11[i];
    }
  }

  /* % Liquid absolute viscosity */
  if (T < 493.57500000000005) {
    T_RB = (T - 273.15) * 9.0 / 5.0 + 32.0;
  } else {
    T_RB = 428.76500000000016;
  }

  dv12[0] = 1.0;
  dv12[1] = T_RB;
  dv12[2] = T_RB * T_RB;
  dv12[3] = rt_powd_snf(T_RB, 3.0);
  dv12[4] = rt_powd_snf(T_RB, 4.0);
  dv12[5] = rt_powd_snf(T_RB, 5.0);
  d2 = 0.0;
  for (i = 0; i < 6; i++) {
    d2 += f_a[i] * dv12[i];
  }

  *mu_l = d2 * exp(p / 6894.757 * (0.0239 + 0.01638 * rt_powd_snf(d2, 0.278)) /
                   1000.0) / 1000.0;

  /* % Liquid thermal conductivity */
  if (p_R[0] <= 1.894) {
    *lambda_l = 1.729578 * (0.07727 - 4.558E-5 * T_RB);
  } else {
    *lambda_l = 1.729578 * (0.07727 - 4.558E-5 * T_RB) * (((17.77 + 0.65 * p_R[0])
      - 7.764 * T_R[0]) - 2.054 * T_R[1] / exp(p_R[0] * 0.2)) / ((18.42 - 7.764 *
      T_R[0]) - 1.681673 * T_R[1]);
  }

  /* % Liquid surface tension */
  if (T_R[0] <= 0.99) {
    *sigma_l = 0.0528806 * rt_powd_snf(1.0 - T_R[0], 1.232);
  } else {
    *sigma_l = 1.0E-5;
  }
}

/*
 * File trailer for GetFuelPropertyN_Dodecane.c
 *
 * [EOF]
 */
