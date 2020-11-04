/*
 * GetPTx.cpp
 *
 * Code generation for function 'GetPTx'
 *
 * C source code generated on: Tue Mar 25 09:23:00 2014
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Get3Wiebe.h"
#include "Get3WiebePara.h"
#include "GetAirDensity.h"
#include "GetAirThermalConduct.h"
#include "GetAirViscosity.h"
#include "GetCompCombGas.h"
#include "GetDensityHCVapor.h"
#include "GetDiffusivityHCVaporToAir.h"
#include "GetEquilGrill.h"
#include "GetFuelPropertyN_Dodecane.h"
#include "GetHTCoeffHTX.h"
#include "GetIdealNozzleFlow.h"
#include "GetPTF.h"
#include "GetPTx.h"
#include "GetT_atm_p.h"
#include "GetThdynCombGasZach.h"
#include "GetThermalPropertyHCVaporReal.h"
#include "GetThermoDynProp.h"
#include "GetViscosityHCVapor.h"
#include "ThdynPack_rtwutil.h"

/* Function Definitions */
void GetPTx(real_T m, const real_T N[12], real_T E, real_T V, real_T T_prev,
            real_T *P, real_T *T, real_T x[12])
{
  real_T y;
  int32_T i;
  real_T MW;
  real_T dT;
  static const real_T b[12] = { 1.00794, 15.9994, 14.0067, 2.01588, 17.00734,
    28.0101, 30.0061, 31.9988, 18.01528, 44.0095, 28.0134, 39.948 };

  real_T R;
  real_T T_err;
  real_T T_qt;
  real_T a_CO2[7];
  real_T a_CO[7];
  real_T a_O2[7];
  real_T a_H2[7];
  real_T a_H2O[7];
  real_T a_OH[7];
  real_T a_O[7];
  real_T a_N2[7];
  real_T a_N[7];
  real_T a_NO[7];
  static const real_T dv32[7] = { 4.6365111, 0.0027414569, -9.9589759E-7,
    1.6038666E-10, -9.1619857E-15, -49024.904, -1.9348955 };

  static const real_T dv33[7] = { 3.0484859, 0.0013517281, -4.8579405E-7,
    7.8853644E-11, -4.6980746E-15, -14266.117, 6.0170977 };

  static const real_T dv34[7] = { 3.66096083, 0.000656365523, -1.41149485E-7,
    2.05797658E-11, -1.29913248E-15, -1215.97725, 3.41536184 };

  static const real_T dv35[7] = { 2.9328305, 0.00082659802, -1.4640057E-7,
    1.5409851E-11, -6.8879615E-16, -813.05582, -1.0243164 };

  static const real_T dv36[7] = { 2.6770389, 0.0029731816, -7.7376889E-7,
    9.4433514E-11, -4.2689991E-15, -29885.894, 6.88255 };

  static const real_T dv37[7] = { 2.83853033, 0.00110741289, -2.94000209E-7,
    4.20698729E-11, -2.4228989E-15, 3697.80808, 5.84494652 };

  static const real_T dv38[7] = { 2.54363697, -2.73162486E-5, -4.1902952E-9,
    4.95481845E-12, -4.79553694E-16, 29226.012, 4.92229457 };

  static const real_T dv39[7] = { 2.95257637, 0.0013969004, -4.92631603E-7,
    7.86010195E-11, -4.60755204E-15, -923.948688, 5.87188762 };

  static const real_T dv40[7] = { 2.4159429, 0.00017489065, -1.1902369E-7,
    3.0226244E-11, -2.0360983E-15, 56133.775, 4.6496095 };

  static const real_T dv41[7] = { 3.26071234, 0.00119101135, -4.29122646E-7,
    6.94481463E-11, -4.03295681E-15, 9921.43132, 6.36900518 };

  static const real_T dv42[7] = { 2.356813, 0.0089841299, -7.1220632E-6,
    2.4573008E-9, -1.4288548E-13, -48371.971, 9.9009035 };

  static const real_T dv43[7] = { 3.5795335, -0.00061035369, 1.0168143E-6,
    9.0700586E-10, -9.0442449E-13, -14344.086, 3.5084093 };

  static const real_T dv44[7] = { 3.78245636, -0.00299673415, 9.847302E-6,
    -9.68129508E-9, 3.24372836E-12, -1063.94356, 3.65767573 };

  static const real_T dv45[7] = { 2.3443029, 0.0079804248, -1.9477917E-5,
    2.0156967E-8, -7.3760289E-12, -917.92413, 0.68300218 };

  static const real_T dv46[7] = { 4.1986352, -0.0020364017, 6.5203416E-6,
    -5.4879269E-9, 1.771968E-12, -30293.726, -0.84900901 };

  static const real_T dv47[7] = { 3.99198424, -0.00240106655, 4.61664033E-6,
    -3.87916306E-9, 1.36319502E-12, 3368.89836, -0.103998477 };

  static const real_T dv48[7] = { 3.1682671, -0.00327931884, 6.64306396E-6,
    -6.12806624E-9, 2.11265971E-12, 29122.2592, 2.05193346 };

  static const real_T dv49[7] = { 3.53100528, -0.000123660988, -5.02999433E-7,
    2.43530612E-9, -1.40881235E-12, -1046.97628, 2.96747038 };

  static const real_T dv50[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 56104.638, 4.1939088
  };

  static const real_T dv51[7] = { 4.21859896, -0.00463988124, 1.10443049E-5,
    -9.34055507E-9, 2.80554874E-12, 9845.09964, 2.28061001 };

  real_T a_H[84];
  static const real_T b_a_H[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 25473.66,
    -0.44668285 };

  static const real_T a_Ar[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, -745.375, 4.3796749 };

  real_T a_gas[7];
  int32_T i6;
  real_T T_arr_Cp[7];
  real_T T_arr_h[7];
  static const real_T dv52[12] = { 26219.035, 29968.7009, 56850.013, 0.0,
    4486.1538, -13293.628, 10977.0882, 0.0, -29084.817, -47328.105, 0.0, 0.0 };

  static const int32_T iv1[12] = { -6197000, -6725000, -6197000, -8467000,
    -9172000, -8671000, -9192000, -8683000, -9904000, -9364000, -8670000,
    -6197000 };

  real_T b_y;

  /* Calculate the pressure, temperature and composition of the combustion gas  */
  /* from given internal energy, volume and mass of each gas specie.  */
  /*  */
  /*    The calculation of the internal energy is based on the NASA 7 */
  /*    coefficient polynomial and T is found by using Newton-Raphson method. */
  /*    The gas is assumed to be idea gas. */
  /*  */
  /*    Input */
  /*        N : No. moles of each gas specie (kmol) */
  /*            N(1) = N_H;   N(2) = N_O;   N(3) = N_N;   N(4) = N_H2;   */
  /*            N(5) = N_OH;  N(6) = N_CO;  N(7) = N_NO;  N(8) = N_O2;   */
  /*            N(9) = N_H2O; N(10) = N_CO2;N(11) = N_N2; N(12) = N_Ar */
  /*        E : Internal energy of the gas */
  /*        V : Volume of the gas */
  /*    Output */
  /*        P : Pressure (Pa) */
  /*        T : Temperature (K) */
  /*        rho : Density (kg/m3) */
  /*        x :  Mole fraction of each gas specie */
  /*            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;  x(5) = x_OH; */
  /*            x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;  x(9) = x_H2O; x(10) = x_CO2; */
  /*            x(11) = x_N2; x(12) = x_Ar */
  /*  */
  /*    Atomic weight calculation from NIST atomic weights */
  /*    Ref : http://physics.nist.gov/cgi-bin/Compositions/stand_alone.pl */
  /*  */
  /*    The NASA 7-coefficients are obtained from the reference: */
  /*    Burcat, A., B. Ruscic, et al. (2005). Third millenium ideal gas and  */
  /*        condensed phase thermochemical database for combustion with updates  */
  /*        from active thermochemical tables, Argonne National Laboratory  */
  /*        Argonne, IL. */
  /*  */
  /*  Created by Kevin Koosup Yum on 29 June */
  /* % */
  /*  in J/kmol/K */
  /* % Calculate the composition in mole fraction and gas constant */
  y = N[0];
  for (i = 0; i < 11; i++) {
    y += N[i + 1];
  }

  MW = 0.0;
  for (i = 0; i < 12; i++) {
    dT = N[i] / y;
    MW += dT * b[i];
    x[i] = dT;
  }

  /*  in kg/kmol */
  R = 8314.4621 / MW;

  /*  in J/kg/K */
  /* % Calculate the temperature */
  *T = T_prev;
  dT = 0.0;
  T_err = 1.0;
  while (T_err > 0.001) {
    *T -= dT;
    dT = *T * *T;
    T_err = rt_powd_snf(*T, 3.0);
    T_qt = rt_powd_snf(*T, 4.0);
    if (*T >= 1000.0) {
      /* Valid upto  */
      for (i = 0; i < 7; i++) {
        a_CO2[i] = dv32[i];

        /* 1000~6000K */
        a_CO[i] = dv33[i];

        /* 1000~6000K */
        a_O2[i] = dv34[i];

        /* 1000~6000K */
        a_H2[i] = dv35[i];

        /* 1000~6000K */
        a_H2O[i] = dv36[i];

        /* 1000~6000K */
        a_OH[i] = dv37[i];

        /* 1000~6000K */
        /* 1000~6000K */
        a_O[i] = dv38[i];

        /* 1000~6000K */
        a_N2[i] = dv39[i];

        /* 1000~6000K */
        a_N[i] = dv40[i];

        /* 1000~6000K */
        a_NO[i] = dv41[i];
      }

      /* 1000~6000K     */
    } else {
      for (i = 0; i < 7; i++) {
        a_CO2[i] = dv42[i];

        /* 300~1000K */
        a_CO[i] = dv43[i];

        /* 300~1000K */
        a_O2[i] = dv44[i];

        /* 300~1000K */
        a_H2[i] = dv45[i];

        /* 300~1000K */
        a_H2O[i] = dv46[i];

        /* 300~1000K */
        a_OH[i] = dv47[i];

        /* 300~1000K     */
        /* 300~1000K */
        a_O[i] = dv48[i];

        /* 300~1000K */
        a_N2[i] = dv49[i];

        /* 300~1000K */
        a_N[i] = dv50[i];

        /* 300~1000K */
        a_NO[i] = dv51[i];
      }

      /* 300~1000K */
    }

    for (i = 0; i < 7; i++) {
      a_H[12 * i] = b_a_H[i];
      a_H[1 + 12 * i] = a_O[i];
      a_H[2 + 12 * i] = a_N[i];
      a_H[3 + 12 * i] = a_H2[i];
      a_H[4 + 12 * i] = a_OH[i];
      a_H[5 + 12 * i] = a_CO[i];
      a_H[6 + 12 * i] = a_NO[i];
      a_H[7 + 12 * i] = a_O2[i];
      a_H[8 + 12 * i] = a_H2O[i];
      a_H[9 + 12 * i] = a_CO2[i];
      a_H[10 + 12 * i] = a_N2[i];
      a_H[11 + 12 * i] = a_Ar[i];
      a_gas[i] = 0.0;
      for (i6 = 0; i6 < 12; i6++) {
        a_gas[i] += x[i6] * a_H[i6 + 12 * i];
      }
    }

    T_arr_Cp[0] = 1.0;
    T_arr_Cp[1] = *T;
    T_arr_Cp[2] = dT;
    T_arr_Cp[3] = T_err;
    T_arr_Cp[4] = T_qt;
    T_arr_Cp[5] = 0.0;
    T_arr_Cp[6] = 0.0;
    T_arr_h[0] = 1.0;
    T_arr_h[1] = *T / 2.0;
    T_arr_h[2] = dT / 3.0;
    T_arr_h[3] = T_err / 4.0;
    T_arr_h[4] = T_qt / 5.0;
    T_arr_h[5] = 1.0 / *T;
    T_arr_h[6] = 0.0;

    /*  (J/kmol / J/kmol/K) */
    /* J/kmol */
    y = 0.0;
    dT = 0.0;
    for (i = 0; i < 12; i++) {
      y += dv52[i] * x[i];
      dT += (real_T)iv1[i] * x[i];
    }

    T_err = R * *T;
    T_qt = 0.0;
    b_y = 0.0;
    for (i = 0; i < 7; i++) {
      T_qt += T_err * a_gas[i] * T_arr_h[i];
      b_y += a_gas[i] * T_arr_Cp[i];
    }

    dT = (((T_qt - (R * y + dT / MW)) - R * *T) - E / m) / (R * (b_y - 1.0));
    T_err = fabs(dT) / *T;
  }

  /* % Calculate the pressure */
  *P = m / V * R * *T;
}

/* End of code generation (GetPTx.cpp) */
