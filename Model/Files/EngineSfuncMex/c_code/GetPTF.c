/*
 * File: GetPTF.c
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
 * Calculate the pressure, temperature and composition of the combustion gas
 * from given internal energy, volume and mass of each gas specie.
 *
 *    The calculation of the internal energy is based on the NASA 7
 *    coefficient polynomial and T is found by using Newton-Raphson method.
 *    The gas is assumed to be idea gas.
 *
 *    Input
 *        N : No. moles of each gas specie (kmol)
 *            N(1) = N_H;   N(2) = N_O;   N(3) = N_N;   N(4) = N_H2;
 *            N(5) = N_OH;  N(6) = N_CO;  N(7) = N_NO;  N(8) = N_O2;
 *            N(9) = N_H2O; N(10) = N_CO2;N(11) = N_N2; N(12) = N_Ar
 *        E : Internal energy of the gas
 *        V : Volume of the gas
 *    Output
 *        P : Pressure (Pa)
 *        T : Temperature (K)
 *        rho : Density (kg/m3)
 *        x :  Mole fraction of each gas specie
 *            x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;  x(5) = x_OH;
 *            x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;  x(9) = x_H2O; x(10) = x_CO2;
 *            x(11) = x_N2; x(12) = x_Ar
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
 * Arguments    : double m
 *                double m_b
 *                double E
 *                double V
 *                double T_prev
 *                double fs
 *                double *p
 *                double *T
 *                double *F
 * Return Type  : void
 */
void GetPTF(double m, double m_b, double E, double V, double T_prev, double fs,
            double *p, double *T, double *F)
{
  double err_p;
  double R;
  double unusedU1b;
  double unusedU1a;
  double unusedU19;
  double unusedU18;
  double unusedU17;
  double unusedU16;
  double uT;
  double unusedU15;
  double unusedU14;
  double unusedU13;
  double unusedU12;
  double unusedU11;
  double u_temp;
  double T_err;

  /* % Calculate F */
  *F = m_b / (m - m_b) / fs;

  /* % Calculate the initial pressure */
  *p = m * 300.0 * T_prev / V;
  err_p = 1.0;
  while (err_p > 0.01) {
    GetThdynCombGasZachV1(*p, T_prev, *F, fs, &err_p, &T_err, &u_temp,
                          &unusedU11, &unusedU12, &unusedU13, &unusedU14,
                          &unusedU15, &uT, &unusedU16, &unusedU17, &unusedU18,
                          &unusedU19, &unusedU1a, &unusedU1b, &R);
    T_err = m * err_p * T_prev / V;
    err_p = fabs(*p - T_err) / *p;
    *p = T_err;
  }

  err_p = 1.0;
  R = 0.0;
  *T = T_prev;
  while (err_p > 0.0001) {
    /* % Calculate the temperature */
    err_p = 0.0;
    T_err = 1.0;
    while (T_err > 0.0001) {
      *T -= err_p;
      GetThdynCombGasZachV1(*p, *T, *F, fs, &R, &err_p, &T_err, &u_temp,
                            &unusedU11, &unusedU12, &unusedU13, &unusedU14,
                            &unusedU15, &uT, &unusedU16, &unusedU17, &unusedU18,
                            &unusedU19, &unusedU1a, &unusedU1b);
      err_p = (u_temp - E / m) / uT;
      T_err = fabs(err_p) / *T;
    }

    T_err = m / V * R * *T;
    err_p = fabs(*p - T_err) / *p;
    *p = T_err;
  }
}

/*
 * File trailer for GetPTF.c
 *
 * [EOF]
 */
