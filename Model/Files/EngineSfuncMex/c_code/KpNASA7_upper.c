/*
 * File: KpNASA7_upper.c
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
#include "KpNASA7_upper.h"

/* Function Definitions */

/*
 * Calculate Kp for dissociation using NASA 7 Polynomial Fit (1000~6000K)
 *  Input
 *        T : Temperature(Kelvin)
 *  Output
 *    Kp_NASA7 (equilibrium constant for 7 reactions)
 *        1; 0.5H2 <-> H              2; 0.5O2 <-> O
 *        3; 0.5N2 <-> N              4; 0.5H2 + 0.5O2 <-> OH
 *        5; 0.5N2 + 0.5O2 <-> NO     6; H2 + 0.5O2 <-> H2O
 *        7; CO + 0.5O2 <-> CO2
 *
 *  Reference
 *    Burcat, A., Ruscic, B., & Laboratory, A. N. (2005). Third millenium
 *    ideal gas and condensed phase thermochemical database for combustion
 *    with updates from active thermochemical tables: Argonne National
 *    Laboratory Argonne, IL.
 *
 *  Revision
 *    -  2012-04-03 Created by Kevin Koosup Yum
 *    -  2013-12-03 Revised to enhance calculation speed and include
 *    temperature derivative by Kevin Koosup Yum
 * Arguments    : double T
 *                double Kp_NASA7[7]
 * Return Type  : void
 */
void KpNASA7_upper(double T, double Kp_NASA7[7])
{
  double T_sq;
  double T_cub;
  double T_quat;
  double dv4[7];
  double dv5[7];
  double dv6[7];
  int i3;
  int i4;
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

  T_sq = T * T;
  T_cub = T_sq * T;
  T_quat = T_cub * T;
  dv4[0] = 1.0;
  dv4[1] = T / 2.0;
  dv4[2] = T_sq / 3.0;
  dv4[3] = T_cub / 4.0;
  dv4[4] = T_quat / 5.0;
  dv4[5] = 1.0 / T;
  dv4[6] = 0.0;
  dv5[0] = log(T);
  dv5[1] = T;
  dv5[2] = T_sq / 2.0;
  dv5[3] = T_cub / 3.0;
  dv5[4] = T_quat / 4.0;
  dv5[5] = 0.0;
  dv5[6] = 1.0;
  for (i3 = 0; i3 < 7; i3++) {
    dv6[i3] = dv4[i3] - dv5[i3];
  }

  for (i3 = 0; i3 < 7; i3++) {
    Kp_NASA7[i3] = 0.0;
    for (i4 = 0; i4 < 7; i4++) {
      Kp_NASA7[i3] += a[i3 + 7 * i4] * dv6[i4];
    }

    Kp_NASA7[i3] = exp(Kp_NASA7[i3]);
  }
}

/*
 * File trailer for KpNASA7_upper.c
 *
 * [EOF]
 */
