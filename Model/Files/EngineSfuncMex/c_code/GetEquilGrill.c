/*
 * File: GetEquilGrill.c
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
 * Calculate the mole fraction of the exhaust gas given P,T,F, fuel and
 * air composition using Grill's Method with some modification for
 * user-defined gas composition
 *
 * Chemical reaction considered
 *   x13(C_n H_m O_l N_k + N_L/F (O_2 + b1 N_2 + b2 Ar + b3 CO + b4 CO_2 +
 *      b5 H_2O + b6 H_2 + b7 H + b8 O + b9 N + b10 OH + b11 NO))
 *            => x1 H + x2 O + x3 N + x4 H2 + x5 OH + x6 x_CO + x7 NO +
 *               x8 O2 + x9 H2O + x10 CO2 + x11 N2 + x12 Ar
 *
 * Ref : Grill, M., A. Schmid, et al. (2007). Calculating the Properties of
 *       User-Defined Working Fluids for Real Working-Process Simulations.
 *       2007 World Congress. Detroit, Michigan, SAE International.
 *  Input
 *    P : Pressure in Pa
 *    T : Temperature in Kelvin
 *    F : air fuel equivalent ratio
 *    Fc : Fuel atomic composition in C, H, O, N
 *         Fc(1) = C, Fc(2) = H, Fc(3) = O, Fc(4) = N
 *    Ac : no. of moles of N2, Ar, CO, CO2,
 *         H2O, H2 as in the ratio of no. of moes of O2
 *         Ac(1) = n_N2/n_O2,  Ac(2) = n_Ar/n_O2,  Ac(3) = n_CO/n_O2
 *         Ac(4) = n_CO2/n_O2, Ac(5) = n_H2O/n_O2, Ac(6) = n_H2/n_O2
 *         Ac(7) = n_H/n_O2,   Ac(8) = n_O/n_O2,   Ac(9) = n_N/n_O2
 *         Ac(10) = n_OH/n_O2, Ac(11) = n_NO/n_O3
 *  Output
 *    x : Composition of exhaust gas in mole fraction of the component
 *        x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;  x(5) = x_OH;
 *        x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;  x(9) = x_H2O; x(10) = x_CO2;
 *        x(11) = x_N2; x(12) = x_Ar
 *
 *
 *  Created by Kevin Koosup Yum on 22 June 2012
 * Arguments    : double P
 *                double T
 *                double F
 *                const double Fc[4]
 *                const double Ac[11]
 *                double x[12]
 * Return Type  : void
 */
void GetEquilGrill(double P, double T, double F, const double Fc[4], const
                   double Ac[11], double x[12])
{
  double N_L_F;
  double r3;
  double N_c_o;
  double N_o_n;
  double N_h_o;
  double N_ar_o;
  int k;
  double N_n_temp;
  double sq_p_o2;
  double T_quat;
  double dv1[7];
  double dv2[7];
  double dv3[7];
  double Kp[7];
  int i1;
  static const double a[49] = { -1.03358475, -0.713156555, -0.939654715,
    0.458365335, 0.04605626, 2.086272015, 0.242455215, 0.00041329901,
    0.0003554990101, 0.00052355955, -0.0003659311185, -0.0001643783885,
    -0.00181840082, -0.00106154604, -7.3200285E-8, -6.63844473E-8,
    -1.272921115E-7, 1.502251815E-7, 1.12232102E-7, 5.567935775E-7,
    4.395287975E-7, 7.7049255E-12, 5.33506445E-12, 9.07426575E-12,
    -2.40750645E-11, -1.985775365E-11, -6.87337801E-11, -7.12431331E-11,
    -3.44398075E-16, -1.70012546E-16, -2.6767772E-16, 1.428934585E-15,
    1.07961455E-15, 2.93063671E-15, 3.81434486E-15, -25880.18791, -29834.000625,
    -56595.749344, -4712.324615, -10991.394289, 28464.849555, 34150.798375,
    -0.06547535, -3.21461365, -1.71366569, -4.6494238, -1.72538045, -6.19918548,
    9.65967412 };

  double N_o_sol;
  double Ksq4;
  double Ksq5;
  double Ksq6;
  double Ksq7;
  double sq_p_h2;
  double sq_p_n2;
  P /= 100000.0;
  N_L_F = ((Fc[1] - 2.0 * Fc[2]) + Fc[0] * 4.0) / ((((((2.0 * Ac[7] - 2.0 * Ac[5])
    - Ac[6]) - 2.0 * Ac[2]) + Ac[9]) + 2.0 * Ac[10]) + 4.0) / F;

  /* r0 in fortran code */
  /* No. of C, R3 in fortran */
  /* No. of H, R4 in fortran */
  r3 = 0.5 * Fc[2] + N_L_F * ((((((1.0 + 0.5 * Ac[2]) + Ac[3]) + Ac[4] / 2.0) +
    0.5 * Ac[7]) + 0.5 * Ac[9]) + 0.5 * Ac[10]);

  /* No. of O2, R in fortran */
  /* No of N2, R1 in fortran */
  /* No of Ar, R2 in fortran */
  N_c_o = (Fc[0] + N_L_F * (Ac[2] + Ac[3])) / (2.0 * r3);
  N_o_n = r3 / (0.5 * Fc[3] + N_L_F * ((Ac[0] + 0.5 * Ac[8]) + 0.5 * Ac[10]));
  N_h_o = (Fc[1] + 2.0 * N_L_F * (((Ac[4] + Ac[5]) + Ac[6] * 0.5) + Ac[9] * 0.5))
    / (2.0 * r3);
  N_ar_o = N_L_F * Ac[1] / (2.0 * r3);
  N_L_F = Ac[0];
  for (k = 0; k < 10; k++) {
    N_L_F += Ac[k + 1];
  }

  N_n_temp = 2.0 * Ac[0] / (N_L_F + 1.0) * P;
  sq_p_o2 = 0.0245 / (F * F);

  /* Calculate Kp for dissociation using NASA 7 Polynomial Fit (1000~6000K) */
  /*  Input  */
  /*        T : Temperature(Kelvin) */
  /*  Output */
  /*    Kp_NASA7 (equilibrium constant for 7 reactions) */
  /*        1; 0.5H2 <-> H              2; 0.5O2 <-> O   */
  /*        3; 0.5N2 <-> N              4; 0.5H2 + 0.5O2 <-> OH */
  /*        5; 0.5N2 + 0.5O2 <-> NO     6; H2 + 0.5O2 <-> H2O */
  /*        7; CO + 0.5O2 <-> CO2 */
  /*  */
  /*  Reference */
  /*    Burcat, A., Ruscic, B., & Laboratory, A. N. (2005). Third millenium */
  /*    ideal gas and condensed phase thermochemical database for combustion */
  /*    with updates from active thermochemical tables: Argonne National */
  /*    Laboratory Argonne, IL. */
  /*  */
  /*  Revision */
  /*    -  2012-04-03 Created by Kevin Koosup Yum */
  /*    -  2013-12-03 Revised to enhance calculation speed and include */
  /*    temperature derivative by Kevin Koosup Yum */
  /*  */
  N_L_F = T * T;
  r3 = N_L_F * T;
  T_quat = r3 * T;
  dv1[0] = 1.0;
  dv1[1] = T / 2.0;
  dv1[2] = N_L_F / 3.0;
  dv1[3] = r3 / 4.0;
  dv1[4] = T_quat / 5.0;
  dv1[5] = 1.0 / T;
  dv1[6] = 0.0;
  dv2[0] = log(T);
  dv2[1] = T;
  dv2[2] = N_L_F / 2.0;
  dv2[3] = r3 / 3.0;
  dv2[4] = T_quat / 4.0;
  dv2[5] = 0.0;
  dv2[6] = 1.0;
  for (k = 0; k < 7; k++) {
    dv3[k] = dv1[k] - dv2[k];
  }

  for (k = 0; k < 7; k++) {
    Kp[k] = 0.0;
    for (i1 = 0; i1 < 7; i1++) {
      Kp[k] += a[k + 7 * i1] * dv3[i1];
    }

    Kp[k] = exp(Kp[k]);
  }

  N_L_F = 1.0;
  r3 = 0.0;
  N_o_sol = N_o_n * N_n_temp;
  while (N_L_F > 0.001) {
    N_n_temp += r3;
    N_o_sol = N_o_n * N_n_temp;
    N_L_F = 1.0;
    r3 = 0.0;
    while (N_L_F > 0.001) {
      sq_p_o2 += r3;
      Ksq4 = Kp[3] * sq_p_o2;
      Ksq5 = Kp[4] * sq_p_o2;
      Ksq6 = Kp[5] * sq_p_o2;
      Ksq7 = Kp[6] * sq_p_o2;
      T_quat = 8.0 * N_h_o * N_o_sol;
      N_L_F = Ksq4 + Kp[0];
      N_L_F = sqrt(T_quat * (1.0 + Ksq6) + N_L_F * N_L_F);
      r3 = 4.0 * (Ksq6 + 1.0);
      sq_p_h2 = -((Kp[0] + Ksq4) - N_L_F) / r3;
      T_quat = Kp[5] * -sq_p_h2 / (r3 / 4.0) - (Kp[3] - 0.5 * (2.0 * Kp[3] *
        (Kp[0] + Ksq4) + Kp[5] * T_quat) / N_L_F) / r3;
      N_L_F = Ksq5 + Kp[2];
      N_L_F = sqrt(N_L_F * N_L_F + 8.0 / N_o_n * N_o_sol);
      sq_p_n2 = ((N_L_F - Ksq5) - Kp[2]) / 4.0;
      r3 = N_c_o * N_o_sol;
      N_L_F = -((((((r3 * (2.0 - 1.0 / (Ksq7 + 1.0)) + 2.0 * (sq_p_o2 * sq_p_o2))
                    + Ksq6 * (sq_p_h2 * sq_p_h2)) + Ksq4 * sq_p_h2) + Kp[1] *
                  sq_p_o2) + Ksq5 * sq_p_n2) - N_o_sol) / ((((((((r3 * Kp[6] /
        ((Ksq7 + 1.0) * (Ksq7 + 1.0)) + 4.0 * sq_p_o2) + Kp[5] * (sq_p_h2 *
        sq_p_h2)) + Ksq6 * 2.0 * sq_p_h2 * T_quat) + Kp[3] * sq_p_h2) + Ksq4 *
        T_quat) + Kp[1]) + Kp[4] * sq_p_n2) + Ksq5 * (((Ksq5 + Kp[2]) * Kp[4] /
        N_L_F - Kp[4]) / 4.0));
      r3 = -0.9 * sq_p_o2;
      if ((N_L_F >= r3) || rtIsNaN(r3)) {
        r3 = N_L_F;
      }

      N_L_F = fabs(r3 / sq_p_o2);
    }

    Ksq4 = Kp[3] * sq_p_o2;
    Ksq5 = Kp[4] * sq_p_o2;
    Ksq6 = Kp[5] * sq_p_o2;
    N_L_F = Ksq4 + Kp[0];
    sq_p_h2 = -((Kp[0] + Ksq4) - sqrt(8.0 * N_h_o * N_o_sol * (1.0 + Ksq6) +
      N_L_F * N_L_F)) / (4.0 * (Ksq6 + 1.0));
    N_L_F = Ksq5 + Kp[2];
    sq_p_n2 = ((sqrt(N_L_F * N_L_F + 8.0 / N_o_n * N_o_sol) - Ksq5) - Kp[2]) /
      4.0;
    N_L_F = N_c_o * N_o_sol / (1.0 / (Kp[6] * sq_p_o2) + 1.0);
    r3 = sq_p_h2 * sq_p_h2;
    r3 = (((((((((((P - N_L_F) - N_L_F / (Kp[6] * sq_p_o2)) - sq_p_o2 * sq_p_o2)
                 - Ksq6 * r3) - r3) - Ksq4 * sq_p_h2) - Kp[0] * sq_p_h2) - Kp[1]
             * sq_p_o2) - sq_p_n2 * sq_p_n2) - Kp[2] * sq_p_n2) - Ksq5 * sq_p_n2)
      - N_ar_o * N_o_sol;
    N_L_F = fabs(r3 / N_n_temp);
  }

  Ksq4 = Kp[3] * sq_p_o2;
  Ksq5 = Kp[4] * sq_p_o2;
  Ksq6 = Kp[5] * sq_p_o2;
  N_L_F = Ksq4 + Kp[0];
  sq_p_h2 = -((Kp[0] + Ksq4) - sqrt(8.0 * N_h_o * N_o_sol * (1.0 + Ksq6) + N_L_F
    * N_L_F)) / (4.0 * (Ksq6 + 1.0));
  N_L_F = Ksq5 + Kp[2];
  sq_p_n2 = ((sqrt(N_L_F * N_L_F + 8.0 / N_o_n * N_o_sol) - Ksq5) - Kp[2]) / 4.0;
  N_L_F = N_c_o * N_o_sol / (1.0 / (Kp[6] * sq_p_o2) + 1.0);
  r3 = sq_p_h2 * sq_p_h2;
  x[0] = Kp[0] * sq_p_h2 / P;
  x[1] = Kp[1] * sq_p_o2 / P;
  x[2] = Kp[2] * sq_p_n2 / P;
  x[3] = r3 / P;
  x[4] = Ksq4 * sq_p_h2 / P;
  x[5] = N_L_F / (Kp[6] * sq_p_o2) / P;
  x[6] = Ksq5 * sq_p_n2 / P;
  x[7] = sq_p_o2 * sq_p_o2 / P;
  x[8] = Ksq6 * r3 / P;
  x[9] = N_L_F / P;
  x[10] = sq_p_n2 * sq_p_n2 / P;
  x[11] = N_ar_o * N_o_sol / P;
}

/*
 * File trailer for GetEquilGrill.c
 *
 * [EOF]
 */
