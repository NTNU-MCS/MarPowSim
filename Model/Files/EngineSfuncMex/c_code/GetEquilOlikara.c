/*
 * File: GetEquilOlikara.c
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
#include "mldivide.h"
#include "KpNASA7_upper.h"

/* Function Definitions */

/*
 * Calculate the mole fraction of the exhaust gas given P,T,F, fuel and
 * air composition using Olikara's Method with some modification for
 * user-defined gas composition.
 * Ref : Olikara, C. and G. Borman (1975). "A Computer Program for
 *       Calculating Properties of Equilibrium Combustion Products with
 *       Some Applications to I.C. Engines." SAE Technical Paper 750468.
 *
 * Chemical reaction considered
 *   x13(C_n H_m O_l N_k + N_L/F (O_2 + b1 N_2 + b2 Ar + b3 CO + b4 CO_2 +
 *      b5 H_2O + b6 H_2 + b7 H + b8 O + b9 N + b10 OH + b11 NO))
 *            => x1 H + x2 O + x3 N + x4 H2 + x5 OH + x6 x_CO + x7 NO +
 *               x8 O2 + x9 H2O + x10 CO2 + x11 N2 + x12 Ar
 *
 *  Input
 *    P : Pressure in bar
 *    T : Temperature in Kelvin
 *    F : air fuel equivalent ratio
 *    Fc : Fuel atomic composition in C, H, O, N
 *         Fc(1) = C, Fc(2) = H, Fc(3) = O, Fc(4) = N
 *    Ac : Cylinder charge composition in no. of moles of N2, Ar, CO, CO2,
 *         H2O, H2 as in the ratio of no. of moes of O2
 *         Ac(1) = n_N2/n_O2,  Ac(2) = n_Ar/n_O2,  Ac(3) = n_CO/n_O2
 *         Ac(4) = n_CO2/n_O2, Ac(5) = n_H2O/n_O2, Ac(6) = n_H2/n_O2
 *         Ac(7) = n_H/n_O2,   Ac(8) = n_O/n_O2,   Ac(9) = n_N/n_O2
 *         Ac(10) = n_OH/n_O2, Ac(11) = n_NO/n_O2
 *  Output
 *    x : Composition of exhaust gas in mole fraction of the component
 *        x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;  x(5) = x_OH;
 *        x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;  x(9) = x_H2O; x(10) = x_CO2;
 *        x(11) = x_N2; x(12) = x_Ar
 *
 *  A global variable "comb" should be defined before calling the function
 *  Assing comb = 0 at the start and end of the combustion and comb = 1
 *  during the combustion
 *
 *  Created by Kevin Koosup Yum on 22 June 2012
 * Arguments    : double P
 *                double T
 *                double F
 *                const double Fc[4]
 *                const double Ac[12]
 *                double x[12]
 * Return Type  : void
 */
void GetEquilOlikara(double P, double T, double F, const double Fc[4], const
                     double Ac[12], double x[12])
{
  double N_L_F;
  double r1;
  double r2;
  double r3;
  double r4;
  double r5;
  double D1;
  double D2;
  double D3;
  double D4;
  double Kp[7];
  double C1;
  double C2;
  double C3;
  double C5;
  double C7;
  double C9;
  double C10;
  double x13;
  double x8;
  double fun1;
  double fun2;
  double fun3;
  double fun4;
  double x8_sqrt;
  int i;
  boolean_T exitg2;
  double x_temp[4];
  int it;
  boolean_T exitg1;
  double y[4];
  double varargin_2[4];
  double b_C1[16];
  double x_temp_new[4];

  /* % Calculate the molecular balance */
  N_L_F = ((Fc[1] - 2.0 * Fc[2]) + Fc[0] * 4.0) / ((((((2.0 * Ac[7] - 2.0 * Ac[5])
    - Ac[6]) - 2.0 * Ac[2]) + Ac[9]) + 2.0 * Ac[10]) + 4.0) / F;

  /* r0 in fortran code */
  r1 = Fc[0] + N_L_F * (Ac[2] + Ac[3]);

  /* No. of C, R3 in fortran */
  r2 = Fc[1] + 2.0 * N_L_F * (((Ac[4] + Ac[5]) + Ac[6] * 0.5) + Ac[9] * 0.5);

  /* No. of H, R4 in fortran */
  r3 = 0.5 * Fc[2] + N_L_F * ((((((1.0 + 0.5 * Ac[2]) + Ac[3]) + Ac[4] / 2.0) +
    0.5 * Ac[7]) + 0.5 * Ac[9]) + 0.5 * Ac[10]);

  /* No. of O2, R in fortran */
  r4 = 0.5 * Fc[3] + N_L_F * ((Ac[0] + 0.5 * Ac[8]) + 0.5 * Ac[10]);

  /* No of N2, R1 in fortran */
  r5 = N_L_F * Ac[1];

  /* No of Ar, R2 in fortran */
  D1 = r2 / r1;
  D2 = 2.0 * r3 / r1;
  D3 = 2.0 * r4 / r1;
  D4 = r5 / r1;
  N_L_F = sqrt(P / 101325.0);

  /* % Calculate the equilibrium constant */
  KpNASA7_upper(T, Kp);
  C1 = Kp[0] / N_L_F;
  C2 = Kp[1] / N_L_F;
  C3 = Kp[2] / N_L_F;
  C5 = Kp[3];
  C7 = Kp[4];
  C9 = Kp[5] * N_L_F;
  C10 = Kp[6] * N_L_F;

  /* % Calculate the initial point */
  if (F <= 1.0) {
    x13 = 1.0 / (((r2 / 4.0 + r3) + r4) + r5);
  } else {
    x13 = 1.0 / (((r1 + r2 / 2.0) + r4) + r5);
  }

  x8 = 10.0;
  N_L_F = 1.0;
  fun1 = 2.0 * C10 * r1;
  fun2 = C9 * r2 / 2.0;
  fun3 = 2.0 / x13;
  fun4 = 2.0 * r3;
  while (N_L_F > 0.0) {
    x8 *= 0.1;
    x8_sqrt = sqrt(x8);
    N_L_F = (((r1 + fun1 * x8_sqrt) / (1.0 + C10 * x8_sqrt) + fun2 * x8_sqrt /
              (1.0 + C9 * x8_sqrt)) + fun3 * x8) - fun4;
  }

  if (N_L_F < 0.0) {
    i = 0;
    exitg2 = false;
    while ((!exitg2) && (i < 20)) {
      x8_sqrt = sqrt(x8);
      N_L_F = 2.0 * C9 * x8_sqrt + 2.0;
      fun1 = C10 * x8_sqrt + 1.0;
      N_L_F = ((((r1 + 2.0 * C10 * r1 * x8_sqrt) / (1.0 + C10 * x8_sqrt) + C9 *
                 r2 * x8_sqrt / (2.0 * (1.0 + C9 * x8_sqrt))) + 2.0 * x8 / x13)
               - 2.0 * r3) / ((((2.0 / x13 - C9 * C9 * r2 / (N_L_F * N_L_F)) +
        C10 * r1 / (x8_sqrt * (C10 * x8_sqrt + 1.0))) + C9 * r2 / (2.0 * x8_sqrt
        * (2.0 * C9 * x8_sqrt + 2.0))) - C10 * (r1 + 2.0 * C10 * r1 * x8_sqrt) /
                              (2.0 * x8_sqrt * (fun1 * fun1)));
      if (N_L_F > x8) {
        x8 *= 0.1;
      } else {
        x8 -= N_L_F;
      }

      if (fabs(N_L_F / x8) < 0.01) {
        exitg2 = true;
      } else {
        i++;
      }
    }
  } else {
    x8_sqrt = sqrt(x8);
  }

  x_temp[0] = r2 * x13 / (2.0 * (1.0 + C9 * x8_sqrt));
  x_temp[1] = r1 * x13 / (1.0 + C10 * x8_sqrt);
  x_temp[2] = x8;
  x_temp[3] = r4 * x13;

  /* % Calculate the equilibrium mole fraction */
  N_L_F = 1.0;
  it = 0;
  exitg1 = false;
  while ((!exitg1) && (N_L_F > 0.001)) {
    it++;
    N_L_F = sqrt(x_temp[0]);
    x8_sqrt = sqrt(x_temp[2]);
    x8 = sqrt(x_temp[3]);
    for (i = 0; i < 4; i++) {
      y[i] = 0.01 * x_temp[i];
    }

    varargin_2[0] = ((((C1 * N_L_F + 2.0 * x_temp[0]) + C5 * N_L_F * x8_sqrt) +
                      2.0 * C9 * x_temp[0] * x8_sqrt) - D1 * x_temp[1]) - D1 *
      C10 * x_temp[1] * x8_sqrt;
    varargin_2[1] = (((((C2 * x8_sqrt + C5 * N_L_F * x8_sqrt) + C9 * x_temp[0] *
                        x8_sqrt) + (1.0 - D2) * x_temp[1]) + (2.0 - D2) * C10 *
                      x_temp[1] * x8_sqrt) + 2.0 * x_temp[2]) + C7 * x8_sqrt *
      x8;
    varargin_2[2] = (((-D3 * x_temp[1] - D3 * C10 * x_temp[1] * x8_sqrt) + C7 *
                      x8_sqrt * x8) + C3 * x8) + 2.0 * x_temp[3];
    varargin_2[3] = ((((((((((x_temp[0] + C9 * x_temp[0] * x8_sqrt) + C1 * N_L_F)
      + C5 * N_L_F * x8_sqrt) + (1.0 + D4) * x_temp[1]) + C2 * x8_sqrt) + C10 *
                         (1.0 + D4) * x_temp[1] * x8_sqrt) + x_temp[2]) + C7 *
                       x8_sqrt * x8) + x_temp[3]) + C3 * x8) - 1.0;
    b_C1[0] = ((C1 / (2.0 * N_L_F) + 2.0 * C9 * x8_sqrt) + C5 * x8_sqrt / (2.0 *
                N_L_F)) + 2.0;
    b_C1[4] = -D1 - C10 * D1 * x8_sqrt;
    b_C1[8] = (C5 * N_L_F / (2.0 * x8_sqrt) + C9 * x_temp[0] / x8_sqrt) - C10 *
      D1 * x_temp[1] / (2.0 * x8_sqrt);
    b_C1[12] = 0.0;
    b_C1[1] = C9 * x8_sqrt + C5 * x8_sqrt / (2.0 * N_L_F);
    b_C1[5] = (x8_sqrt * C10 * (2.0 - D2) - D2) + 1.0;
    b_C1[9] = ((((C2 + C5 * N_L_F) + C7 * x8) + x_temp[1] * C10 * (2.0 - D2)) +
               C9 * x_temp[0]) / (2.0 * x8_sqrt) + 2.0;
    b_C1[13] = C7 * x8_sqrt / (2.0 * x8);
    b_C1[2] = 0.0;
    b_C1[6] = -D3 - C10 * D3 * x8_sqrt;
    b_C1[10] = (C7 * x8 - C10 * D3 * x_temp[1]) / (2.0 * x8_sqrt);
    b_C1[14] = (C3 + C7 * x8_sqrt) / (2.0 * x8) + 2.0;
    b_C1[3] = ((C1 + C5 * x8_sqrt) / (2.0 * N_L_F) + C9 * x8_sqrt) + 1.0;
    b_C1[7] = (D4 + C10 * x8_sqrt * (D4 + 1.0)) + 1.0;
    b_C1[11] = ((((C5 * N_L_F + C7 * x8) + C9 * x_temp[0]) + C10 * x_temp[1] *
                 (D4 + 1.0)) + C2) / (2.0 * x8_sqrt) + 1.0;
    b_C1[15] = (C3 + C7 * x8_sqrt) / (2.0 * x8) + 1.0;
    mldivide(b_C1, varargin_2);
    for (i = 0; i < 4; i++) {
      N_L_F = x_temp[i] - varargin_2[i];
      if ((y[i] >= N_L_F) || rtIsNaN(N_L_F)) {
        x8 = y[i];
      } else {
        x8 = N_L_F;
      }

      varargin_2[i] = N_L_F;
      y[i] = fabs((x_temp[i] - x8) / x_temp[i]);
      x_temp_new[i] = x8;
    }

    N_L_F = y[0];
    for (i = 0; i < 3; i++) {
      N_L_F += y[i + 1];
    }

    for (i = 0; i < 4; i++) {
      x_temp[i] = x_temp_new[i];
    }

    if (it >= 1000) {
      exitg1 = true;
    }
  }

  x13 = C1 * sqrt(x_temp[0]);
  fun4 = C2 * sqrt(x_temp[2]);
  fun1 = C3 * sqrt(x_temp[3]);
  fun2 = Kp[3] * sqrt(x_temp[0]) * sqrt(x_temp[2]);
  fun3 = Kp[4] * sqrt(x_temp[2]) * sqrt(x_temp[3]);
  r5 = C9 * x_temp[0] * sqrt(x_temp[2]);
  N_L_F = C10 * x_temp[1] * sqrt(x_temp[2]);
  x8 = x_temp[0];
  for (i = 0; i < 3; i++) {
    x8 += x_temp[i + 1];
  }

  x[0] = x13;
  x[1] = fun4;
  x[2] = fun1;
  x[3] = x_temp[0];
  x[4] = fun2;
  x[5] = x_temp[1];
  x[6] = fun3;
  x[7] = x_temp[2];
  x[8] = r5;
  x[9] = N_L_F;
  x[10] = x_temp[3];
  x[11] = 1.0 - (((((((x13 + fun4) + fun1) + fun2) + fun3) + r5) + N_L_F) + x8);
}

/*
 * File trailer for GetEquilOlikara.c
 *
 * [EOF]
 */
