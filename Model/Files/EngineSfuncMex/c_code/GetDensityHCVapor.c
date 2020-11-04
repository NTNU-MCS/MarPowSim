/*
 * File: GetDensityHCVapor.c
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
#include "rdivide.h"

/* Function Definitions */

/*
 * The function calculates the density of hydrocarbon vapor.The calculation
 * is following the procedure 6B1.8 from API Technical Book of Petroleum
 * Refining
 *  Input
 *    p : Pressure of the gas in Pa
 *    T : Temperature of the gas in Kelvin
 *    p_CR : Critical pressure of the gas in Pa
 *    T_CR : Critical temperature of the gas on K
 *    MW : Molecular weight of the gas in kg/kmol
 *    W : Acentric factor of the gas
 *  Output
 *    rho_v : density of the gas in kg/m3
 *  Comment
 *    Error is less than 1% in average. But near critical temperature, it may
 *    increase upto 30%. Due to iteration problem, this routine is only
 *    applicable from 1.0 for reduced temperature and pressure upto 20.
 *  Reference
 *    Technical Data Book :6B1.8 - Petroleum Refining(1985) Americal Petroleum Institute.
 *  Created by Kevin Koosup Yum, 27 August 2013
 * Arguments    : double p
 *                double T
 *                double p_CR
 *                double T_CR
 *                double MW
 *                double W
 *                double rho_v_data[]
 *                int rho_v_size[1]
 * Return Type  : void
 */
void GetDensityHCVapor(double p, double T, double p_CR, double T_CR, double MW,
  double W, double rho_v_data[], int rho_v_size[1])
{
  double p_R;
  double T_R;
  double v_R_ideal;
  double TT_data[4];
  int ar;
  double z_data[4];
  double B_data_idx_0;
  int ib;
  static const double B[4] = { 0.1181193, -0.265728, -0.15479, -0.030323 };

  double b_TT_data[3];
  double b_z_data[3];
  double C_data_idx_0;
  static const double b_B[3] = { 0.0236744, -0.0186984, 0.0 };

  double D;
  double B_h_data_idx_0;
  static const double c_B[4] = { 0.2026579, -0.331511, -0.027655, -0.203488 };

  double C_h_data_idx_0;
  static const double d_B[3] = { 0.0313385, -0.0503618, 0.016901 };

  double z;
  double D_h;
  double VV[6];
  double V_R_error;
  double V_R;
  double V_R_temp;
  double c43;
  double varargin_1;

  /* Universal gas constant J/kmol-K */
  p_R = p / p_CR;
  T_R = T / T_CR;
  rho_v_size[0] = 1;
  v_R_ideal = rdivide(8314.0 * T, p) / (8314.0 * T_CR / p_CR);
  for (ar = 0; ar < 4; ar++) {
    TT_data[ar] = 0.0;
  }

  TT_data[0] = 1.0;
  for (ar = 0; ar < 3; ar++) {
    TT_data[ar + 1] = TT_data[ar] * T_R;
  }

  for (ar = 0; ar < 4; ar++) {
    z_data[ar] = 1.0 / TT_data[ar];
  }

  B_data_idx_0 = 0.0;
  ar = -1;
  for (ib = 0; ib + 1 < 5; ib++) {
    if (B[ib] != 0.0) {
      B_data_idx_0 += B[ib] * z_data[ar + 1];
    }

    ar++;
  }

  for (ar = 0; ar < 2; ar++) {
    b_TT_data[ar] = TT_data[ar];
  }

  b_TT_data[2] = TT_data[3];
  for (ar = 0; ar < 3; ar++) {
    b_z_data[ar] = 1.0 / b_TT_data[ar];
  }

  C_data_idx_0 = 0.0;
  ar = -1;
  for (ib = 0; ib + 1 < 4; ib++) {
    if (b_B[ib] != 0.0) {
      C_data_idx_0 += b_B[ib] * b_z_data[ar + 1];
    }

    ar++;
  }

  D = 1.55488E-5 + rdivide(6.23689E-5, T_R);
  for (ar = 0; ar < 4; ar++) {
    z_data[ar] = 1.0 / TT_data[ar];
  }

  B_h_data_idx_0 = 0.0;
  ar = -1;
  for (ib = 0; ib + 1 < 5; ib++) {
    if (c_B[ib] != 0.0) {
      B_h_data_idx_0 += c_B[ib] * z_data[ar + 1];
    }

    ar++;
  }

  for (ar = 0; ar < 2; ar++) {
    b_TT_data[ar] = TT_data[ar];
  }

  b_TT_data[2] = TT_data[3];
  for (ar = 0; ar < 3; ar++) {
    b_z_data[ar] = 1.0 / b_TT_data[ar];
  }

  C_h_data_idx_0 = 0.0;
  ar = -1;
  for (ib = 0; ib + 1 < 4; ib++) {
    if (d_B[ib] != 0.0) {
      C_h_data_idx_0 += d_B[ib] * b_z_data[ar + 1];
    }

    ar++;
  }

  z = 7.40336E-6 / T_R;
  D_h = 4.8736E-5 + 7.40336E-6 / T_R;
  for (ar = 0; ar < 6; ar++) {
    VV[ar] = 0.0;
  }

  /* Simple fluid calculation */
  V_R_error = 1.0;
  V_R = v_R_ideal * 0.8;
  while (V_R_error > 0.001) {
    VV[0] = V_R;
    for (ar = 0; ar < 5; ar++) {
      VV[ar + 1] = VV[ar] * V_R;
    }

    V_R_error = 0.042724 / TT_data[3] / VV[1];
    V_R_temp = 0.060167 / VV[1];
    c43 = exp(-0.060167 / VV[1]);
    varargin_1 = 0.1 * V_R;
    V_R_error = V_R - (p_R / T_R * V_R - ((((1.0 + B_data_idx_0 / V_R) +
      C_data_idx_0 / VV[1]) + D / VV[4]) + V_R_error * (0.65392 + V_R_temp) *
      c43)) / (p_R / T_R - (((((-B_data_idx_0 / VV[1] - 2.0 * C_data_idx_0 / VV
      [2]) - 5.0 * D / VV[5]) + -0.085448 / TT_data[3] / VV[2] * (0.65392 +
      V_R_temp) * c43) + V_R_error * (-0.120334 / VV[2]) * c43) + V_R_error *
                (0.65392 + V_R_temp) * (0.120334 / VV[2] * c43)));
    if ((varargin_1 >= V_R_error) || rtIsNaN(V_R_error)) {
      V_R_temp = varargin_1;
    } else {
      V_R_temp = V_R_error;
    }

    V_R_error = fabs(V_R_temp - V_R) / V_R;
    V_R = V_R_temp;
  }

  B_data_idx_0 = p_R * V_R / T_R;

  /* Heavy Reference fluid calculation */
  V_R = v_R_ideal * 0.8;
  V_R_error = 1.0;
  while (V_R_error > 0.001) {
    VV[0] = V_R;
    for (ar = 0; ar < 5; ar++) {
      VV[ar + 1] = VV[ar] * V_R;
    }

    V_R_error = 0.041577 / TT_data[3] / VV[1];
    V_R_temp = 0.03754 / VV[1];
    c43 = exp(-0.03754 / VV[1]);
    varargin_1 = 0.1 * V_R;
    V_R_error = V_R - (p_R / T_R * V_R - ((((1.0 + B_h_data_idx_0 / V_R) +
      C_h_data_idx_0 / VV[1]) + D_h / VV[4]) + V_R_error * (1.226 + V_R_temp) *
      c43)) / (p_R / T_R - (((((-B_h_data_idx_0 / VV[1] - 2.0 * C_h_data_idx_0 /
      VV[2]) - 5.0 * (4.8736E-5 + z) / VV[5]) + -0.083154 / TT_data[3] / VV[2] *
                  (1.226 + V_R_temp) * c43) + V_R_error * (-0.07508 / VV[2]) *
      c43) + V_R_error * (1.226 + V_R_temp) * (0.07508 / VV[2] * c43)));
    if ((varargin_1 >= V_R_error) || rtIsNaN(V_R_error)) {
      V_R_temp = varargin_1;
    } else {
      V_R_temp = V_R_error;
    }

    V_R_error = fabs(V_R_temp - V_R) / V_R;
    V_R = V_R_temp;
  }

  rho_v_data[0] = 1.0 / ((B_data_idx_0 + W / 0.3978 * (p_R * V_R / T_R -
    B_data_idx_0)) * 8314.0 * T / p) * MW;
}

/*
 * File trailer for GetDensityHCVapor.c
 *
 * [EOF]
 */
