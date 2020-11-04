/*
 * File: GetThermoDynProp.c
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
 * Calculate the specific thermodynamic properties of the gas based on the
 * pressure, temperature and the composition based on the 7-coefficient NASA
 * polynomials. Valid in the temperature range 300~6000K.
 *    Input
 *        P : Pressure in Pa
 *        T : Temperature in Kelvin
 *        x : Composition of the gas
 *            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;
 *            x(5) = x_OH;  x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;
 *            x(9) = x_H2O; x(10) = x_CO2;x(11) = x_N2; x(12) = x_Ar
 *    Output
 *        R : Gas constant
 *        Cp : Specific heat capacity at constant pressure (J/kg/K)
 *        Cv : Specific heat capacity at constant volume (J/kg/K)
 *        u : Specific internal energy (J/kg)
 *        h : Specific enthalpy (J/kg)
 *        s : Specific entropy (J/kg/T)
 *        rho : Density of the gas (kg/m3)
 *
 *    Atomic weight calculation from NIST atomic weights
 *    Ref : http://physics.nist.gov/cgi-bin/Compositions/stand_alone.pl
 *
 *    The NASA 7-coefficients are obtained from the reference:
 *    Burcat, A., B. Ruscic, et al. (2005). Third millenium ideal gas and
 *        condensed phase thermochemical database for combustion with updates
 *        from active thermochemical tables, Argonne National Laboratory
 *        Argonne, IL.
 *
 *  Created by Kevin Koosup Yum on 29 June
 * %
 * Arguments    : double P
 *                double T
 *                const double x[12]
 *                double *R
 *                double *Cp
 *                double *Cv
 *                double *u
 *                double *ht
 *                double *s
 *                double *rho
 * Return Type  : void
 */
void GetThermoDynProp(double P, double T, const double x[12], double *R, double *
                      Cp, double *Cv, double *u, double *ht, double *s, double
                      *rho)
{
  double T_sq;
  double T_cb;
  double T_qt;
  double d4;
  int k;
  static const double b[12] = { 1.00794, 15.9994, 14.0067, 2.01588, 17.00734,
    28.0101, 30.0061, 31.9988, 18.01528, 44.0095, 28.0134, 39.948 };

  double a_CO2[7];
  double a_CO[7];
  double a_O2[7];
  double a_H2[7];
  double a_H2O[7];
  double a_OH[7];
  double a_O[7];
  double a_N2[7];
  double a_N[7];
  double a_NO[7];
  static const double dv13[7] = { 4.6365111, 0.0027414569, -9.9589759E-7,
    1.6038666E-10, -9.1619857E-15, -49024.904, -1.9348955 };

  static const double dv14[7] = { 3.0484859, 0.0013517281, -4.8579405E-7,
    7.8853644E-11, -4.6980746E-15, -14266.117, 6.0170977 };

  static const double dv15[7] = { 3.66096083, 0.000656365523, -1.41149485E-7,
    2.05797658E-11, -1.29913248E-15, -1215.97725, 3.41536184 };

  static const double dv16[7] = { 2.9328305, 0.00082659802, -1.4640057E-7,
    1.5409851E-11, -6.8879615E-16, -813.05582, -1.0243164 };

  static const double dv17[7] = { 2.6770389, 0.0029731816, -7.7376889E-7,
    9.4433514E-11, -4.2689991E-15, -29885.894, 6.88255 };

  static const double dv18[7] = { 2.83853033, 0.00110741289, -2.94000209E-7,
    4.20698729E-11, -2.4228989E-15, 3697.80808, 5.84494652 };

  static const double dv19[7] = { 2.54363697, -2.73162486E-5, -4.1902952E-9,
    4.95481845E-12, -4.79553694E-16, 29226.012, 4.92229457 };

  static const double dv20[7] = { 2.95257637, 0.0013969004, -4.92631603E-7,
    7.86010195E-11, -4.60755204E-15, -923.948688, 5.87188762 };

  static const double dv21[7] = { 2.4159429, 0.00017489065, -1.1902369E-7,
    3.0226244E-11, -2.0360983E-15, 56133.775, 4.6496095 };

  static const double dv22[7] = { 3.26071234, 0.00119101135, -4.29122646E-7,
    6.94481463E-11, -4.03295681E-15, 9921.43132, 6.36900518 };

  static const double dv23[7] = { 2.356813, 0.0089841299, -7.1220632E-6,
    2.4573008E-9, -1.4288548E-13, -48371.971, 9.9009035 };

  static const double dv24[7] = { 3.5795335, -0.00061035369, 1.0168143E-6,
    9.0700586E-10, -9.0442449E-13, -14344.086, 3.5084093 };

  static const double dv25[7] = { 3.78245636, -0.00299673415, 9.847302E-6,
    -9.68129508E-9, 3.24372836E-12, -1063.94356, 3.65767573 };

  static const double dv26[7] = { 2.3443029, 0.0079804248, -1.9477917E-5,
    2.0156967E-8, -7.3760289E-12, -917.92413, 0.68300218 };

  static const double dv27[7] = { 4.1986352, -0.0020364017, 6.5203416E-6,
    -5.4879269E-9, 1.771968E-12, -30293.726, -0.84900901 };

  static const double dv28[7] = { 3.99198424, -0.00240106655, 4.61664033E-6,
    -3.87916306E-9, 1.36319502E-12, 3368.89836, -0.103998477 };

  static const double dv29[7] = { 3.1682671, -0.00327931884, 6.64306396E-6,
    -6.12806624E-9, 2.11265971E-12, 29122.2592, 2.05193346 };

  static const double dv30[7] = { 3.53100528, -0.000123660988, -5.02999433E-7,
    2.43530612E-9, -1.40881235E-12, -1046.97628, 2.96747038 };

  static const double dv31[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 56104.638, 4.1939088
  };

  static const double dv32[7] = { 4.21859896, -0.00463988124, 1.10443049E-5,
    -9.34055507E-9, 2.80554874E-12, 9845.09964, 2.28061001 };

  double a_H[84];
  double dv33[7];
  static const double b_a_H[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 25473.66,
    -0.44668285 };

  static const double a_Ar[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, -745.375, 4.3796749 };

  double a_gas[7];
  int i6;
  double y;
  double dv34[7];
  double d5;
  double d6;
  static const double a[12] = { 26219.035, 29968.7009, 56850.013, 0.0, 4486.1538,
    -13293.628, 10977.0882, 0.0, -29084.817, -47328.105, 0.0, 0.0 };

  static const int b_a[12] = { -6197000, -6725000, -6197000, -8467000, -9172000,
    -8671000, -9192000, -8683000, -9904000, -9364000, -8670000, -6197000 };

  double dv35[7];
  T_sq = T * T;
  T_cb = rt_powd_snf(T, 3.0);
  T_qt = rt_powd_snf(T, 4.0);

  /*  in J/kmol/K */
  /* % Calculate R */
  /*  kg/kmol */
  d4 = 0.0;
  for (k = 0; k < 12; k++) {
    d4 += x[k] * b[k];
  }

  /*  in kg/kmol */
  *R = 8314.4621 / d4;

  /*  in J/kg/K */
  /* % */
  if (T >= 1000.0) {
    /* Valid upto  */
    for (k = 0; k < 7; k++) {
      a_CO2[k] = dv13[k];

      /* 1000~6000K */
      a_CO[k] = dv14[k];

      /* 1000~6000K */
      a_O2[k] = dv15[k];

      /* 1000~6000K */
      a_H2[k] = dv16[k];

      /* 1000~6000K */
      a_H2O[k] = dv17[k];

      /* 1000~6000K */
      a_OH[k] = dv18[k];

      /* 1000~6000K */
      /* 1000~6000K */
      a_O[k] = dv19[k];

      /* 1000~6000K */
      a_N2[k] = dv20[k];

      /* 1000~6000K */
      a_N[k] = dv21[k];

      /* 1000~6000K */
      a_NO[k] = dv22[k];
    }

    /* 1000~6000K     */
  } else {
    for (k = 0; k < 7; k++) {
      a_CO2[k] = dv23[k];

      /* 300~1000K */
      a_CO[k] = dv24[k];

      /* 300~1000K */
      a_O2[k] = dv25[k];

      /* 300~1000K */
      a_H2[k] = dv26[k];

      /* 300~1000K */
      a_H2O[k] = dv27[k];

      /* 300~1000K */
      a_OH[k] = dv28[k];

      /* 300~1000K     */
      /* 300~1000K */
      a_O[k] = dv29[k];

      /* 300~1000K */
      a_N2[k] = dv30[k];

      /* 300~1000K */
      a_N[k] = dv31[k];

      /* 300~1000K */
      a_NO[k] = dv32[k];
    }

    /* 300~1000K */
  }

  /* % Calculate Cp & Cv */
  /* k = x ./ M_mw'; */
  /* a_gas_Cp = k*A; */
  dv33[0] = 1.0;
  dv33[1] = T;
  dv33[2] = T_sq;
  dv33[3] = T_cb;
  dv33[4] = T_qt;
  dv33[5] = 0.0;
  dv33[6] = 0.0;
  for (k = 0; k < 7; k++) {
    a_H[12 * k] = b_a_H[k];
    a_H[1 + 12 * k] = a_O[k];
    a_H[2 + 12 * k] = a_N[k];
    a_H[3 + 12 * k] = a_H2[k];
    a_H[4 + 12 * k] = a_OH[k];
    a_H[5 + 12 * k] = a_CO[k];
    a_H[6 + 12 * k] = a_NO[k];
    a_H[7 + 12 * k] = a_O2[k];
    a_H[8 + 12 * k] = a_H2O[k];
    a_H[9 + 12 * k] = a_CO2[k];
    a_H[10 + 12 * k] = a_N2[k];
    a_H[11 + 12 * k] = a_Ar[k];
    a_gas[k] = 0.0;
    for (i6 = 0; i6 < 12; i6++) {
      a_gas[k] += x[i6] * a_H[i6 + 12 * k];
    }

    a_CO2[k] = a_gas[k] * dv33[k];
  }

  y = a_CO2[0];
  for (k = 0; k < 6; k++) {
    y += a_CO2[k + 1];
  }

  *Cp = *R * y;

  /* J/kmol/K */
  *Cv = *Cp - *R;

  /* J/kmol/K */
  /* % Calculate h and u */
  /*            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;   */
  /*            x(5) = x_OH;  x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;   */
  /*            x(9) = x_H2O; x(10) = x_CO2;x(11) = x_N2; x(12) = x_Ar */
  /*  (J/kmol / J/kmol/K) */
  /* J/kmol */
  dv34[0] = 1.0;
  dv34[1] = T / 2.0;
  dv34[2] = T_sq / 3.0;
  dv34[3] = T_cb / 4.0;
  dv34[4] = T_qt / 5.0;
  dv34[5] = 1.0 / T;
  dv34[6] = 0.0;
  for (k = 0; k < 7; k++) {
    a_CO2[k] = a_gas[k] * dv34[k];
  }

  y = a_CO2[0];
  for (k = 0; k < 6; k++) {
    y += a_CO2[k + 1];
  }

  d5 = 0.0;
  d6 = 0.0;
  for (k = 0; k < 12; k++) {
    d5 += a[k] * x[k];
    d6 += (double)b_a[k] * x[k];
  }

  *ht = *R * T * y - (*R * d5 + d6 / d4);
  *u = *ht - *R * T;

  /* % Calculate s */
  dv35[0] = log(T);
  dv35[1] = T;
  dv35[2] = T_sq / 2.0;
  dv35[3] = T_cb / 3.0;
  dv35[4] = T_qt / 4.0;
  dv35[5] = 0.0;
  dv35[6] = 1.0;
  for (k = 0; k < 7; k++) {
    a_CO2[k] = a_gas[k] * dv35[k];
  }

  y = a_CO2[0];
  for (k = 0; k < 6; k++) {
    y += a_CO2[k + 1];
  }

  *s = *R * y;

  /* % Calculate rho */
  *rho = P / (*R * T);
}

/*
 * File trailer for GetThermoDynProp.c
 *
 * [EOF]
 */
