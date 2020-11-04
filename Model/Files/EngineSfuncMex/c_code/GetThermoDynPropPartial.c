/*
 * File: GetThermoDynPropPartial.c
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
 * Calculate the specific thermodynamic properties of the gas and
 * partial derivatives of them based on the pressure, temperature and the
 * composition based on the 7-coefficient NASA polynomials. Valid in the
 * temperature range 300~6000K.
 *    Input
 *        P : Pressure in Pa
 *        T : Temperature in Kelvin
 *        x : Composition of the gas
 *            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;
 *            x(5) = x_OH;  x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;
 *            x(9) = x_H2O; x(10) = x_CO2;x(11) = x_N2; x(12) = x_Ar
 *    Output
 *        R : Gas constant (J/kgK)
 *        h : Specific enthalpy (J/kg)
 *        s : Specific entropy (J/kg/K)
 *        u : Specific internal energy (J/kg)
 *        RP : partial derivative(PD) of R w.r.t. pressure
 *        RT : partial derivative(PD) of R w.r.t. temperature
 *        RF : partial derivative(PD) of R w.r.t. F
 *        uP : partial derivative(PD) of u w.r.t. pressure
 *        uT : partial derivative(PD) of u w.r.t. temperature
 *        uF : partial derivative(PD) of u w.r.t. F
 *        sP : partial derivative(PD) of s w.r.t. pressure
 *        sT : partial derivative(PD) of s w.r.t. temperature
 *        sF : partial derivative(PD) of s w.r.t. F
 *        Cp : Specific heat capacity at constant pressure (J/kg/K)
 *        Cv : Specific heat capacity at constant volume (J/kg/K)
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
 *                const double dx[36]
 *                double *R
 *                double *h
 *                double *s
 *                double *u
 *                double *RP
 *                double *RT
 *                double *RF
 *                double *uP
 *                double *uT
 *                double *uF
 *                double *sP
 *                double *sT
 *                double *sF
 *                double *Cp
 *                double *Cv
 * Return Type  : void
 */
void GetThermoDynPropPartial(double P, double T, const double x[12], const
  double dx[36], double *R, double *h, double *s, double *u, double *RP, double *
  RT, double *RF, double *uP, double *uT, double *uF, double *sP, double *sT,
  double *sF, double *Cp, double *Cv)
{
  double T_sq;
  double T_cb;
  double T_qt;
  double d15;
  int i9;
  static const double b[12] = { 1.00794, 15.9994, 14.0067, 2.01588, 17.00734,
    28.0101, 30.0061, 31.9988, 18.01528, 44.0095, 28.0134, 39.948 };

  double dR[3];
  double dMW[3];
  int i10;
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
  static const double dv60[7] = { 4.6365111, 0.0027414569, -9.9589759E-7,
    1.6038666E-10, -9.1619857E-15, -49024.904, -1.9348955 };

  static const double dv61[7] = { 3.0484859, 0.0013517281, -4.8579405E-7,
    7.8853644E-11, -4.6980746E-15, -14266.117, 6.0170977 };

  static const double dv62[7] = { 3.66096083, 0.000656365523, -1.41149485E-7,
    2.05797658E-11, -1.29913248E-15, -1215.97725, 3.41536184 };

  static const double dv63[7] = { 2.9328305, 0.00082659802, -1.4640057E-7,
    1.5409851E-11, -6.8879615E-16, -813.05582, -1.0243164 };

  static const double dv64[7] = { 2.6770389, 0.0029731816, -7.7376889E-7,
    9.4433514E-11, -4.2689991E-15, -29885.894, 6.88255 };

  static const double dv65[7] = { 2.83853033, 0.00110741289, -2.94000209E-7,
    4.20698729E-11, -2.4228989E-15, 3697.80808, 5.84494652 };

  static const double dv66[7] = { 2.54363697, -2.73162486E-5, -4.1902952E-9,
    4.95481845E-12, -4.79553694E-16, 29226.012, 4.92229457 };

  static const double dv67[7] = { 2.95257637, 0.0013969004, -4.92631603E-7,
    7.86010195E-11, -4.60755204E-15, -923.948688, 5.87188762 };

  static const double dv68[7] = { 2.4159429, 0.00017489065, -1.1902369E-7,
    3.0226244E-11, -2.0360983E-15, 56133.775, 4.6496095 };

  static const double dv69[7] = { 3.26071234, 0.00119101135, -4.29122646E-7,
    6.94481463E-11, -4.03295681E-15, 9921.43132, 6.36900518 };

  static const double dv70[7] = { 2.356813, 0.0089841299, -7.1220632E-6,
    2.4573008E-9, -1.4288548E-13, -48371.971, 9.9009035 };

  static const double dv71[7] = { 3.5795335, -0.00061035369, 1.0168143E-6,
    9.0700586E-10, -9.0442449E-13, -14344.086, 3.5084093 };

  static const double dv72[7] = { 3.78245636, -0.00299673415, 9.847302E-6,
    -9.68129508E-9, 3.24372836E-12, -1063.94356, 3.65767573 };

  static const double dv73[7] = { 2.3443029, 0.0079804248, -1.9477917E-5,
    2.0156967E-8, -7.3760289E-12, -917.92413, 0.68300218 };

  static const double dv74[7] = { 4.1986352, -0.0020364017, 6.5203416E-6,
    -5.4879269E-9, 1.771968E-12, -30293.726, -0.84900901 };

  static const double dv75[7] = { 3.99198424, -0.00240106655, 4.61664033E-6,
    -3.87916306E-9, 1.36319502E-12, 3368.89836, -0.103998477 };

  static const double dv76[7] = { 3.1682671, -0.00327931884, 6.64306396E-6,
    -6.12806624E-9, 2.11265971E-12, 29122.2592, 2.05193346 };

  static const double dv77[7] = { 3.53100528, -0.000123660988, -5.02999433E-7,
    2.43530612E-9, -1.40881235E-12, -1046.97628, 2.96747038 };

  static const double dv78[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 56104.638, 4.1939088
  };

  static const double dv79[7] = { 4.21859896, -0.00463988124, 1.10443049E-5,
    -9.34055507E-9, 2.80554874E-12, 9845.09964, 2.28061001 };

  static const double a_H[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 25473.66, -0.44668285
  };

  double A[84];
  static const double a_Ar[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, -745.375, 4.3796749 };

  double da_gas[21];
  int i11;
  double T_arr_Cp[7];
  double T_arr_h[7];
  double y;
  static const double a[12] = { 26219.035, 29968.7009, 56850.013, 0.0, 4486.1538,
    -13293.628, 10977.0882, 0.0, -29084.817, -47328.105, 0.0, 0.0 };

  double b_y;
  static const int b_b[12] = { -6197000, -6725000, -6197000, -8467000, -9172000,
    -8671000, -9192000, -8683000, -9904000, -9364000, -8670000, -6197000 };

  double c;
  double b_dx[3];
  double c_dx[3];
  double dh_298_0[3];
  static const double c_b[12] = { 26219.035, 29968.7009, 56850.013, 0.0,
    4486.1538, -13293.628, 10977.0882, 0.0, -29084.817, -47328.105, 0.0, 0.0 };

  static const int b_a[12] = { -6197000, -6725000, -6197000, -8467000, -9172000,
    -8671000, -9192000, -8683000, -9904000, -9364000, -8670000, -6197000 };

  double dv80[7];
  (void)P;
  T_sq = T * T;
  T_cb = T_sq * T;
  T_qt = T_cb * T;

  /*  in J/kmol/K */
  /* % Calculate R */
  /*  kg/kmol */
  d15 = 0.0;
  for (i9 = 0; i9 < 12; i9++) {
    d15 += x[i9] * b[i9];
  }

  /*  in kg/kmol */
  /*  Partial derivative of MW in p,T,F */
  *R = 8314.4621 / d15;
  for (i9 = 0; i9 < 3; i9++) {
    dMW[i9] = 0.0;
    for (i10 = 0; i10 < 12; i10++) {
      dMW[i9] += dx[i10 + 12 * i9] * b[i10];
    }

    /*  in J/kg/K */
    dR[i9] = -*R * dMW[i9] / d15;
  }

  *RP = dR[0];
  *RT = dR[1];
  *RF = dR[2];

  /* % */
  if (T >= 1000.0) {
    /* Valid upto  */
    for (i9 = 0; i9 < 7; i9++) {
      a_CO2[i9] = dv60[i9];

      /* 1000~6000K */
      a_CO[i9] = dv61[i9];

      /* 1000~6000K */
      a_O2[i9] = dv62[i9];

      /* 1000~6000K */
      a_H2[i9] = dv63[i9];

      /* 1000~6000K */
      a_H2O[i9] = dv64[i9];

      /* 1000~6000K */
      a_OH[i9] = dv65[i9];

      /* 1000~6000K */
      /* 1000~6000K */
      a_O[i9] = dv66[i9];

      /* 1000~6000K */
      a_N2[i9] = dv67[i9];

      /* 1000~6000K */
      a_N[i9] = dv68[i9];

      /* 1000~6000K */
      a_NO[i9] = dv69[i9];
    }

    /* 1000~6000K     */
  } else {
    for (i9 = 0; i9 < 7; i9++) {
      a_CO2[i9] = dv70[i9];

      /* 300~1000K */
      a_CO[i9] = dv71[i9];

      /* 300~1000K */
      a_O2[i9] = dv72[i9];

      /* 300~1000K */
      a_H2[i9] = dv73[i9];

      /* 300~1000K */
      a_H2O[i9] = dv74[i9];

      /* 300~1000K */
      a_OH[i9] = dv75[i9];

      /* 300~1000K     */
      /* 300~1000K */
      a_O[i9] = dv76[i9];

      /* 300~1000K */
      a_N2[i9] = dv77[i9];

      /* 300~1000K */
      a_N[i9] = dv78[i9];

      /* 300~1000K */
      a_NO[i9] = dv79[i9];
    }

    /* 300~1000K */
  }

  for (i9 = 0; i9 < 7; i9++) {
    A[12 * i9] = a_H[i9];
    A[1 + 12 * i9] = a_O[i9];
    A[2 + 12 * i9] = a_N[i9];
    A[3 + 12 * i9] = a_H2[i9];
    A[4 + 12 * i9] = a_OH[i9];
    A[5 + 12 * i9] = a_CO[i9];
    A[6 + 12 * i9] = a_NO[i9];
    A[7 + 12 * i9] = a_O2[i9];
    A[8 + 12 * i9] = a_H2O[i9];
    A[9 + 12 * i9] = a_CO2[i9];
    A[10 + 12 * i9] = a_N2[i9];
    A[11 + 12 * i9] = a_Ar[i9];
    a_CO2[i9] = 0.0;
    for (i10 = 0; i10 < 12; i10++) {
      a_CO2[i9] += x[i10] * A[i10 + 12 * i9];
    }
  }

  for (i9 = 0; i9 < 3; i9++) {
    for (i10 = 0; i10 < 7; i10++) {
      da_gas[i9 + 3 * i10] = 0.0;
      for (i11 = 0; i11 < 12; i11++) {
        da_gas[i9 + 3 * i10] += dx[i11 + 12 * i9] * A[i11 + 12 * i10];
      }
    }
  }

  /* % Calculate Cp & Cv */
  T_arr_Cp[0] = 1.0;
  T_arr_Cp[1] = T;
  T_arr_Cp[2] = T_sq;
  T_arr_Cp[3] = T_cb;
  T_arr_Cp[4] = T_qt;
  T_arr_Cp[5] = 0.0;
  T_arr_Cp[6] = 0.0;

  /* k = x ./ M_mw'; */
  /* a_gas_Cp = k*A; */
  *Cp = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    *Cp += *R * a_CO2[i9] * T_arr_Cp[i9];
  }

  /* J/kmol/K */
  *Cv = *Cp - *R;

  /* J/kmol/K */
  /* % Calculate h and u */
  /*  (J/kmol / J/kmol/K) */
  /* J/kmol */
  T_arr_h[0] = T;
  T_arr_h[1] = T_sq / 2.0;
  T_arr_h[2] = T_cb / 3.0;
  T_arr_h[3] = T_qt / 4.0;
  T_arr_h[4] = T_qt * T / 5.0;
  T_arr_h[5] = 1.0;
  T_arr_h[6] = 0.0;

  /*            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;   */
  /*            x(5) = x_OH;  x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;   */
  /*            x(9) = x_H2O; x(10) = x_CO2;x(11) = x_N2; x(12) = x_Ar */
  y = 0.0;
  for (i9 = 0; i9 < 12; i9++) {
    y += a[i9] * x[i9];
  }

  b_y = 0.0;
  for (i9 = 0; i9 < 12; i9++) {
    b_y += x[i9] * (double)b_b[i9];
  }

  c = d15 * d15;
  for (i9 = 0; i9 < 3; i9++) {
    b_dx[i9] = 0.0;
    for (i10 = 0; i10 < 12; i10++) {
      b_dx[i9] += dx[i10 + 12 * i9] * c_b[i10];
    }

    c_dx[i9] = 0.0;
    for (i10 = 0; i10 < 12; i10++) {
      c_dx[i9] += dx[i10 + 12 * i9] * (double)b_b[i10];
    }

    dh_298_0[i9] = (dR[i9] * y + *R * b_dx[i9]) + (c_dx[i9] * d15 - b_y * dMW[i9])
      / c;
  }

  c = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    c += a_CO2[i9] * T_arr_h[i9];
  }

  b_y = 0.0;
  y = 0.0;
  for (i9 = 0; i9 < 12; i9++) {
    b_y += a[i9] * x[i9];
    y += (double)b_a[i9] * x[i9];
  }

  *h = *R * c - (*R * b_y + y / d15);
  *u = *h - *R * T;
  d15 = 0.0;
  c = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    d15 += a_CO2[i9] * T_arr_h[i9];
    c += da_gas[3 * i9] * T_arr_h[i9];
  }

  *uP = ((dR[0] * d15 + *R * c) - dh_298_0[0]) - dR[0] * T;
  d15 = 0.0;
  c = 0.0;
  b_y = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    d15 += a_CO2[i9] * T_arr_h[i9];
    c += da_gas[1 + 3 * i9] * T_arr_h[i9];
    b_y += *R * a_CO2[i9] * T_arr_Cp[i9];
  }

  *uT = ((((dR[1] * d15 + *R * c) + b_y) - dh_298_0[1]) - *R) - dR[1] * T;
  d15 = 0.0;
  c = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    d15 += a_CO2[i9] * T_arr_h[i9];
    c += da_gas[2 + 3 * i9] * T_arr_h[i9];
  }

  *uF = ((dR[2] * d15 + *R * c) - dh_298_0[2]) - dR[2] * T;

  /* % Calculate s */
  T_arr_Cp[0] = log(T);
  T_arr_Cp[1] = T;
  T_arr_Cp[2] = T_sq / 2.0;
  T_arr_Cp[3] = T_cb / 3.0;
  T_arr_Cp[4] = T_qt / 4.0;
  T_arr_Cp[5] = 0.0;
  T_arr_Cp[6] = 1.0;
  *s = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    *s += *R * a_CO2[i9] * T_arr_Cp[i9];
  }

  d15 = 0.0;
  c = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    d15 += dR[0] * a_CO2[i9] * T_arr_Cp[i9];
    c += *R * da_gas[3 * i9] * T_arr_Cp[i9];
  }

  *sP = d15 + c;
  d15 = 0.0;
  c = 0.0;
  dv80[0] = 1.0 / T;
  dv80[1] = 1.0;
  dv80[2] = T;
  dv80[3] = T_sq;
  dv80[4] = T_cb;
  dv80[5] = 0.0;
  dv80[6] = 0.0;
  b_y = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    d15 += dR[1] * a_CO2[i9] * T_arr_Cp[i9];
    c += *R * da_gas[1 + 3 * i9] * T_arr_Cp[i9];
    b_y += *R * a_CO2[i9] * dv80[i9];
  }

  *sT = (d15 + c) + b_y;
  d15 = 0.0;
  c = 0.0;
  for (i9 = 0; i9 < 7; i9++) {
    d15 += dR[2] * a_CO2[i9] * T_arr_Cp[i9];
    c += *R * da_gas[2 + 3 * i9] * T_arr_Cp[i9];
  }

  *sF = d15 + c;

  /* % Calculate rho */
}

/*
 * File trailer for GetThermoDynPropPartial.c
 *
 * [EOF]
 */
