/*
 * File: GetAirDensity.c
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
 * The function calculates the air density based on equation of state
 * proposed by Kadoya.
 *  Input
 *    p : pressure in Pa
 *    T : Temperature in K
 *  Output
 *    rho_a : density of the air
 *  Validity : 300~2000K and upto 100MPa.
 *  Ref
 *    Kadoya, K., N. Matsunaga, et al. (1985). "Viscosity and Thermal
 *    Conductivity of Dry Air in the Gaseous Phase." J. Phys. Chem. Ref. Data 14(4).
 *  Created by Kevin Koosup Yum (NTNU) on 23 May, 2013
 * Arguments    : double p
 *                double T
 * Return Type  : double
 */
double GetAirDensity(double p, double T)
{
  double rho_a;
  double rho_ideal;
  double TT[6];
  int i;
  double rho_error;
  double rho;
  double tau;
  static const double a[6] = { -3.4600925E-8, -4.5776295E-7, -3.4932E-6,
    -3.9930515E-5, 4.4292095E-5, -4.5598149999999997E-7 };

  static const double b_a[4] = { 1.0518311175000001E-9, -2.3202380725E-9,
    1.8055242775E-9, 5.028921625E-10 };

  double varargin_1;

  /* Universal Gas constant J/(molK) */
  rho_ideal = p / T / 8.314;

  /*  (K) */
  /*  (10e-6m3/mol) */
  for (i = 0; i < 6; i++) {
    TT[i] = 0.0;
  }

  rho_error = 1.0;
  rho = rho_ideal;
  tau = T / 340.0;
  TT[0] = 1.0 / rt_powd_snf(tau, 4.0);
  for (i = 0; i < 5; i++) {
    TT[i + 1] = TT[i] * tau;
  }

  while (rho_error > 0.001) {
    tau = 0.0;
    for (i = 0; i < 6; i++) {
      tau += a[i] * TT[i];
    }

    rho_error = 0.0;
    for (i = 0; i < 4; i++) {
      rho_error += b_a[i] * TT[1 + i];
    }

    varargin_1 = 0.1 * rho;
    tau = rho + -(((rho_ideal / rho - 1.0) - tau * rho) - rho_error * (rho * rho))
      / ((-rho_ideal / (rho * rho) - tau) - 2.0 * rho_error * rho);
    if ((varargin_1 >= tau) || rtIsNaN(tau)) {
      tau = varargin_1;
    }

    rho_error = fabs(tau - rho) / rho;
    rho = tau;
  }

  rho_a = rho * 28.97 / 1000.0;

  /* end */
  return rho_a;
}

/*
 * File trailer for GetAirDensity.c
 *
 * [EOF]
 */
