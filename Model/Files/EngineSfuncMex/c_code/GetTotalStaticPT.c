/*
 * File: GetTotalStaticPT.c
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
 * Calculates the total pressure, total and static temperature based from the
 * measurements.
 *  Input
 *    - p : Pressure measurement (static) [Pa]
 *    - T : Temperature measurement [K]
 *    - x : Composition of the gas in mole fraction (12x1)
 *    - RF : Recovery factor for temperature measurement [K]
 *    - mDot : mass flow at the measurement [kg/s]
 *    - Area : Sectional area at the measurement [m2]
 *    - c : velocity of the gas at the measurement [m/s], optional
 *  Output
 *    - p0 : Total pressure [Pa]
 *    - T0 : Total temperature [K];
 *    - Ts : Static temperature [K];
 * Arguments    : double varargin_1
 *                double varargin_2
 *                const double varargin_3[12]
 *                double varargin_4
 *                double varargin_5
 *                double varargin_6
 *                double varargin_7
 *                double *p0
 *                double *T0
 *                double *Ts
 * Return Type  : void
 */
void GetTotalStaticPT(double varargin_1, double varargin_2, const double
                      varargin_3[12], double varargin_4, double varargin_5,
                      double varargin_6, double varargin_7, double *p0, double
                      *T0, double *Ts)
{
  double err;
  double rhoTemp;
  double T_qt;
  double T_cb;
  double T_sq;
  double unusedUa;
  double p0Temp;
  double deltaT;
  double cSq;
  double TsTemp;
  int ixstart;
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
  static const double dv81[7] = { 4.6365111, 0.0027414569, -9.9589759E-7,
    1.6038666E-10, -9.1619857E-15, -49024.904, -1.9348955 };

  static const double dv82[7] = { 3.0484859, 0.0013517281, -4.8579405E-7,
    7.8853644E-11, -4.6980746E-15, -14266.117, 6.0170977 };

  static const double dv83[7] = { 3.66096083, 0.000656365523, -1.41149485E-7,
    2.05797658E-11, -1.29913248E-15, -1215.97725, 3.41536184 };

  static const double dv84[7] = { 2.9328305, 0.00082659802, -1.4640057E-7,
    1.5409851E-11, -6.8879615E-16, -813.05582, -1.0243164 };

  static const double dv85[7] = { 2.6770389, 0.0029731816, -7.7376889E-7,
    9.4433514E-11, -4.2689991E-15, -29885.894, 6.88255 };

  static const double dv86[7] = { 2.83853033, 0.00110741289, -2.94000209E-7,
    4.20698729E-11, -2.4228989E-15, 3697.80808, 5.84494652 };

  static const double dv87[7] = { 2.54363697, -2.73162486E-5, -4.1902952E-9,
    4.95481845E-12, -4.79553694E-16, 29226.012, 4.92229457 };

  static const double dv88[7] = { 2.95257637, 0.0013969004, -4.92631603E-7,
    7.86010195E-11, -4.60755204E-15, -923.948688, 5.87188762 };

  static const double dv89[7] = { 2.4159429, 0.00017489065, -1.1902369E-7,
    3.0226244E-11, -2.0360983E-15, 56133.775, 4.6496095 };

  static const double dv90[7] = { 3.26071234, 0.00119101135, -4.29122646E-7,
    6.94481463E-11, -4.03295681E-15, 9921.43132, 6.36900518 };

  static const double dv91[7] = { 2.356813, 0.0089841299, -7.1220632E-6,
    2.4573008E-9, -1.4288548E-13, -48371.971, 9.9009035 };

  static const double dv92[7] = { 3.5795335, -0.00061035369, 1.0168143E-6,
    9.0700586E-10, -9.0442449E-13, -14344.086, 3.5084093 };

  static const double dv93[7] = { 3.78245636, -0.00299673415, 9.847302E-6,
    -9.68129508E-9, 3.24372836E-12, -1063.94356, 3.65767573 };

  static const double dv94[7] = { 2.3443029, 0.0079804248, -1.9477917E-5,
    2.0156967E-8, -7.3760289E-12, -917.92413, 0.68300218 };

  static const double dv95[7] = { 4.1986352, -0.0020364017, 6.5203416E-6,
    -5.4879269E-9, 1.771968E-12, -30293.726, -0.84900901 };

  static const double dv96[7] = { 3.99198424, -0.00240106655, 4.61664033E-6,
    -3.87916306E-9, 1.36319502E-12, 3368.89836, -0.103998477 };

  static const double dv97[7] = { 3.1682671, -0.00327931884, 6.64306396E-6,
    -6.12806624E-9, 2.11265971E-12, 29122.2592, 2.05193346 };

  static const double dv98[7] = { 3.53100528, -0.000123660988, -5.02999433E-7,
    2.43530612E-9, -1.40881235E-12, -1046.97628, 2.96747038 };

  static const double dv99[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 56104.638, 4.1939088
  };

  static const double dv100[7] = { 4.21859896, -0.00463988124, 1.10443049E-5,
    -9.34055507E-9, 2.80554874E-12, 9845.09964, 2.28061001 };

  double a_H[84];
  double dv101[7];
  static const double b_a_H[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, 25473.66,
    -0.44668285 };

  static const double a_Ar[7] = { 2.5, 0.0, 0.0, 0.0, 0.0, -745.375, 4.3796749 };

  double a_gas[7];
  int ix;
  double dv102[7];
  double x[3];
  double y[3];
  boolean_T exitg1;
  (void)varargin_5;
  (void)varargin_6;
  err = 1.0;
  GetThermoDynProp(varargin_1, varargin_2, varargin_3, &deltaT, &p0Temp,
                   &unusedUa, &T_sq, &T_cb, &T_qt, &rhoTemp);
  cSq = varargin_7 * varargin_7;
  *p0 = varargin_1 + 0.5 * rhoTemp * cSq;
  deltaT = 0.5 * cSq / p0Temp;
  *T0 = varargin_2 + (1.0 - varargin_4) * deltaT;
  *Ts = varargin_2 - varargin_4 * deltaT;
  while (err > 0.001) {
    p0Temp = *p0;
    err = *T0;
    TsTemp = *Ts;

    /* Calculate the specific thermodynamic properties of the gas based on the */
    /* pressure, temperature and the composition based on the 7-coefficient NASA */
    /* polynomials. Valid in the temperature range 300~6000K. */
    /*    Input */
    /*        P : Pressure in Pa */
    /*        T : Temperature in Kelvin */
    /*        x : Composition of the gas */
    /*            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;   */
    /*            x(5) = x_OH;  x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;   */
    /*            x(9) = x_H2O; x(10) = x_CO2;x(11) = x_N2; x(12) = x_Ar */
    /*    Output */
    /*        R : Gas constant  */
    /*        Cp : Specific heat capacity at constant pressure (J/kg/K)  */
    /*        Cv : Specific heat capacity at constant volume (J/kg/K) */
    /*        u : Specific internal energy (J/kg) */
    /*        h : Specific enthalpy (J/kg) */
    /*        s : Specific entropy (J/kg/T) */
    /*        rho : Density of the gas (kg/m3) */
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
    T_sq = *Ts * *Ts;
    T_cb = rt_powd_snf(*Ts, 3.0);
    T_qt = rt_powd_snf(*Ts, 4.0);

    /*  in J/kmol/K */
    /* % Calculate R */
    /*  kg/kmol */
    /*  in kg/kmol */
    unusedUa = 0.0;
    for (ixstart = 0; ixstart < 12; ixstart++) {
      unusedUa += varargin_3[ixstart] * b[ixstart];
    }

    unusedUa = 8314.4621 / unusedUa;

    /*  in J/kg/K */
    /* % */
    if (*Ts >= 1000.0) {
      /* Valid upto  */
      for (ixstart = 0; ixstart < 7; ixstart++) {
        a_CO2[ixstart] = dv81[ixstart];

        /* 1000~6000K */
        a_CO[ixstart] = dv82[ixstart];

        /* 1000~6000K */
        a_O2[ixstart] = dv83[ixstart];

        /* 1000~6000K */
        a_H2[ixstart] = dv84[ixstart];

        /* 1000~6000K */
        a_H2O[ixstart] = dv85[ixstart];

        /* 1000~6000K */
        a_OH[ixstart] = dv86[ixstart];

        /* 1000~6000K */
        /* 1000~6000K */
        a_O[ixstart] = dv87[ixstart];

        /* 1000~6000K */
        a_N2[ixstart] = dv88[ixstart];

        /* 1000~6000K */
        a_N[ixstart] = dv89[ixstart];

        /* 1000~6000K */
        a_NO[ixstart] = dv90[ixstart];
      }

      /* 1000~6000K     */
    } else {
      for (ixstart = 0; ixstart < 7; ixstart++) {
        a_CO2[ixstart] = dv91[ixstart];

        /* 300~1000K */
        a_CO[ixstart] = dv92[ixstart];

        /* 300~1000K */
        a_O2[ixstart] = dv93[ixstart];

        /* 300~1000K */
        a_H2[ixstart] = dv94[ixstart];

        /* 300~1000K */
        a_H2O[ixstart] = dv95[ixstart];

        /* 300~1000K */
        a_OH[ixstart] = dv96[ixstart];

        /* 300~1000K     */
        /* 300~1000K */
        a_O[ixstart] = dv97[ixstart];

        /* 300~1000K */
        a_N2[ixstart] = dv98[ixstart];

        /* 300~1000K */
        a_N[ixstart] = dv99[ixstart];

        /* 300~1000K */
        a_NO[ixstart] = dv100[ixstart];
      }

      /* 300~1000K */
    }

    /* % Calculate Cp & Cv */
    /* k = x ./ M_mw'; */
    /* a_gas_Cp = k*A; */
    dv101[0] = 1.0;
    dv101[1] = *Ts;
    dv101[2] = T_sq;
    dv101[3] = T_cb;
    dv101[4] = T_qt;
    dv101[5] = 0.0;
    dv101[6] = 0.0;
    for (ixstart = 0; ixstart < 7; ixstart++) {
      a_H[12 * ixstart] = b_a_H[ixstart];
      a_H[1 + 12 * ixstart] = a_O[ixstart];
      a_H[2 + 12 * ixstart] = a_N[ixstart];
      a_H[3 + 12 * ixstart] = a_H2[ixstart];
      a_H[4 + 12 * ixstart] = a_OH[ixstart];
      a_H[5 + 12 * ixstart] = a_CO[ixstart];
      a_H[6 + 12 * ixstart] = a_NO[ixstart];
      a_H[7 + 12 * ixstart] = a_O2[ixstart];
      a_H[8 + 12 * ixstart] = a_H2O[ixstart];
      a_H[9 + 12 * ixstart] = a_CO2[ixstart];
      a_H[10 + 12 * ixstart] = a_N2[ixstart];
      a_H[11 + 12 * ixstart] = a_Ar[ixstart];
      a_gas[ixstart] = 0.0;
      for (ix = 0; ix < 12; ix++) {
        a_gas[ixstart] += varargin_3[ix] * a_H[ix + 12 * ixstart];
      }

      a_CO2[ixstart] = a_gas[ixstart] * dv101[ixstart];
    }

    rhoTemp = a_CO2[0];
    for (ixstart = 0; ixstart < 6; ixstart++) {
      rhoTemp += a_CO2[ixstart + 1];
    }

    /* J/kmol/K */
    /* J/kmol/K */
    /* % Calculate h and u */
    /*            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;   */
    /*            x(5) = x_OH;  x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;   */
    /*            x(9) = x_H2O; x(10) = x_CO2;x(11) = x_N2; x(12) = x_Ar */
    /*  (J/kmol / J/kmol/K) */
    /* J/kmol */
    /* % Calculate s */
    dv102[0] = log(*Ts);
    dv102[1] = *Ts;
    dv102[2] = T_sq / 2.0;
    dv102[3] = T_cb / 3.0;
    dv102[4] = T_qt / 4.0;
    dv102[5] = 0.0;
    dv102[6] = 1.0;
    for (ixstart = 0; ixstart < 7; ixstart++) {
      a_CO2[ixstart] = a_gas[ixstart] * dv102[ixstart];
    }

    deltaT = a_CO2[0];
    for (ixstart = 0; ixstart < 6; ixstart++) {
      deltaT += a_CO2[ixstart + 1];
    }

    /* % Calculate rho */
    *p0 = varargin_1 + 0.5 * (unusedUa * deltaT) * cSq;
    deltaT = 0.5 * cSq / (unusedUa * rhoTemp);
    *T0 = varargin_2 + (1.0 - varargin_4) * deltaT;
    *Ts = varargin_2 - varargin_4 * deltaT;
    x[0] = (*p0 - p0Temp) / p0Temp;
    x[1] = (*T0 - err) / err;
    x[2] = (*Ts - TsTemp) / TsTemp;
    for (ixstart = 0; ixstart < 3; ixstart++) {
      y[ixstart] = fabs(x[ixstart]);
    }

    ixstart = 1;
    err = y[0];
    if (rtIsNaN(y[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix < 4)) {
        ixstart = ix;
        if (!rtIsNaN(y[ix - 1])) {
          err = y[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < 3) {
      while (ixstart + 1 < 4) {
        if (y[ixstart] > err) {
          err = y[ixstart];
        }

        ixstart++;
      }
    }
  }
}

/*
 * File trailer for GetTotalStaticPT.c
 *
 * [EOF]
 */
