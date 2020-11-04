/*
 * File: GetHTCoeffHTX.c
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
 * Calculate the heat transfer coeff of convectivity for charge air cooler
 *  exchanger (Cross-flow type)
 *  Input
 *    p : pressure (Pa)
 *    m_dot : mass flow (kg/s)
 *    T : temperature (K), should be mean value of the air and coolant
 *    D : Diameter of tube (coolant side)
 *    A : Effective sectional area of air path
 *  Output
 *    alpha : Heat transfer coefficient of convectivity (W/m2K)
 *
 *  Created by Kevin Koosup Yum (25 March 2014)
 * Arguments    : double p
 *                double m_dot
 *                double T
 *                double D
 *                double A
 * Return Type  : double
 */
double GetHTCoeffHTX(double p, double m_dot, double T, double D, double A)
{
  double rho_a;
  double mu_a;
  double lambda;
  double unusedUe;
  double unusedUd;
  double d3;
  double unusedUc;
  double unusedUb;
  double unusedUa;
  double unusedU9;
  double unusedU8;
  double unusedU7;
  double unusedU6;
  double unusedU5;
  double unusedU4;
  double unusedU3;
  double unusedU2;
  double unusedU1;
  double Re_D;
  rho_a = GetAirDensity(p, T);
  mu_a = GetAirViscosity(rho_a, T);
  lambda = GetAirThermalConduct(rho_a, T);
  GetThdynCombGasZachV1(p, T, 0.0, 0.0683, &Re_D, &unusedU1, &unusedU2,
                        &unusedU3, &unusedU4, &unusedU5, &unusedU6, &unusedU7,
                        &unusedU8, &unusedU9, &unusedUa, &unusedUb, &unusedUc,
                        &d3, &unusedUd, &unusedUe);

  /* % */
  Re_D = m_dot / rho_a / A * D * rho_a / mu_a;
  rho_a = mu_a * d3 / lambda;

  /* { */
  /* Nu_D1 = zeros(m,1); */
  /* idx = find(Re_D <= 4); */
  /* Nu_D1(idx) = 0.989*Re_D(idx).^0.330.*Pr(idx); */
  /* idx = find(Re_D <= 40 & Re_D > 4); */
  /* Nu_D1(idx) = 0.911*Re_D(idx).^0.385.*Pr(idx); */
  /* idx = find(Re_D <= 4000 & Re_D > 40); */
  /* Nu_D1(idx) = 0.683*Re_D(idx).^0.466.*Pr(idx); */
  /* idx = find(Re_D <= 40000 & Re_D > 4000); */
  /* Nu_D1(idx) = 0.193*Re_D(idx).^0.618.*Pr(idx); */
  /* idx = find(Re_D <= 400000 & Re_D > 40000); */
  /* Nu_D1(idx) = 0.027*Re_D(idx).^0.805.*Pr(idx); */
  /* alpha1 = Nu_D1.*lambda/D; */
  /* } */
  return (0.3 + 0.62 * sqrt(Re_D) * rt_powd_snf(rho_a, 0.333) / rt_powd_snf(1.0
           + rt_powd_snf(0.4 / rho_a, 0.666), 0.25) * rt_powd_snf(1.0 +
           rt_powd_snf(Re_D / 282000.0, 0.625), 0.8)) * lambda / D;
}

/*
 * File trailer for GetHTCoeffHTX.c
 *
 * [EOF]
 */
