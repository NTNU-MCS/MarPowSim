/*
 * File: GetAirThermalConduct.c
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
 * The function calculates the air thermal conductivity based on correlation
 * proposed by Kadoya.
 *  Theory
 *    \lambda\left(T,\rho\right) = \Lambda \left(\lambda_0(T_r) + \Delta\lambda(\rho_r)\right)
 *    \lambda_0(T_r) = C_1 T + C_{0.5} T^{0.5} + \sum_{i=0}^{-4} C_i T_r^i
 *    \Delta\lambda(\rho_r) = \sum_{i=1}^{5} B_i rho_r^i
 *    T_r = T/T_{ref}     \rho_r = rho/rho_{ref}
 *
 *  Input
 *    rho : pressure in kg/m3
 *    T : Temperature in K
 * Arguments    : double rho
 *                double T
 * Return Type  : double
 */
double GetAirThermalConduct(double rho, double T)
{
  double rho_r;
  double T_r;
  double TT[6];
  int i;
  double rhor[5];
  static const double b[6] = { 0.239503, 1.0, -1.92615, 2.00383, -1.07553,
    0.229414 };

  double d0;
  static const double b_b[5] = { 0.402287, 0.356603, -0.163159, 0.138059,
    -0.020172 };

  /*  Output */
  /*    lambda_a : thermal conductivity of the air in W/(mK) */
  /*  Validity : +-1% in high temperature (>370K) */
  /*  Ref */
  /*    Kadoya, K., N. Matsunaga, et al. (1985). "Viscosity and Thermal  */
  /*    Conductivity of Dry Air in the Gaseous Phase." J. Phys. Chem. Ref. Data 14(4). */
  /*  Created by Kevin Koosup Yum (NTNU) on 29 August, 2013 */
  rho_r = rho / 314.3;
  T_r = T / 132.5;
  for (i = 0; i < 6; i++) {
    TT[i] = 0.0;
  }

  for (i = 0; i < 5; i++) {
    rhor[i] = 0.0;
  }

  TT[0] = T_r;
  rhor[0] = rho_r;
  for (i = 0; i < 4; i++) {
    TT[i + 1] = TT[i] / T_r;
    rhor[i + 1] = rhor[i] * rho_r;
  }

  TT[5] = TT[4] / T_r;
  rho_r = 0.0;
  for (i = 0; i < 6; i++) {
    rho_r += TT[i] * b[i];
  }

  d0 = 0.0;
  for (i = 0; i < 5; i++) {
    d0 += rhor[i] * b_b[i];
  }

  return 0.0259778 * ((rho_r + 0.00649768 * sqrt(T_r)) + d0);
}

/*
 * File trailer for GetAirThermalConduct.c
 *
 * [EOF]
 */
