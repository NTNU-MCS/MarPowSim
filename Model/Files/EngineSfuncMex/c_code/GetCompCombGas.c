/*
 * File: GetCompCombGas.c
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
#include "GetCompleteComb.h"
#include "ThdynPack_rtwutil.h"

/* Function Definitions */

/*
 * Calculate the composition of combustion gas with either no dissociation
 * or full dissociation or partially dissociation;
 *  For full dissociation case, the composition is calculated by
 *  GetEquilGrill(). For complete combustion case, the composition is
 *  calculated by GetCompleteComb(). For the partial dissociation case, the
 *  blending function is used. The transition region
 *
 *  Ref : Valland, H. (2012). Computation of Thermodynamic Properties of
 *        Combustion Products and Fuel-Air Processes. TMR9  Internal
 *        Combustion Engines, Advanced Course Lecture Note. Trondheim,
 *        Department of Marine Technology, NTNU.
 *
 *  Input
 *    P : Pressure of the gas (Pa)
 *    T : Temperature of the gas (K)
 *    F : fuel air equivalent ratio
 *    Fc : Fuel atomic composition in C, H, O, N
 *         Fc(1) = C, Fc(2) = H, Fc(3) = O, Fc(4) = N
 *    x_air : Cylinder charge composition in mole fraction
 *        x_air(1) = x_H;   x_air(2) = x_O;   x_air(3) = x_N;
 *        x_air(4) = x_H2;  x_air(5) = x_OH;  x_air(6) = x_CO;
 *        x_air(7) = x_NO;  x_air(8) = x_O2;  x_air(9) = x_H2O;from
 *        x_air(10) = x_CO2;x_air(11) = x_N2; x_air(12) = x_Ar
 *    T_L : Lower bound of the transition temperature range (K)
 *    T_H : Upper bound of the transition temperature range (K)
 *  Output
 *    x : Composition of exhaust gas in mole fraction of the component
 *        x(1) = x_H;   x(2) = x_O;   x(3) = x_N;   x(4) = x_H2;  x(5) = x_OH;
 *        x(6) = x_CO;  x(7) = x_NO;  x(8) = x_O2;  x(9) = x_H2O; x(10) = x_CO2;
 *        x(11) = x_N2; x(12) = x_Ar
 *
 *  Created by Kevin Koosup Yum on 28 June 2012
 * Arguments    : double P
 *                double T
 *                double F
 *                double T_L
 *                double T_H
 *                const double Fc[4]
 *                const double x_air[12]
 *                double x[12]
 * Return Type  : void
 */
void GetCompCombGas(double P, double T, double F, double T_L, double T_H, const
                    double Fc[4], const double x_air[12], double x[12])
{
  double Ac[11];
  double q;
  double dv0[12];
  int i0;
  Ac[0] = x_air[10] / x_air[7];
  Ac[1] = x_air[11] / x_air[7];
  Ac[2] = x_air[5] / x_air[7];
  Ac[3] = x_air[9] / x_air[7];
  Ac[4] = x_air[8] / x_air[7];
  Ac[5] = x_air[3] / x_air[7];
  Ac[6] = x_air[0] / x_air[7];
  Ac[7] = x_air[1] / x_air[7];
  Ac[8] = x_air[2] / x_air[7];
  Ac[9] = x_air[4] / x_air[7];
  Ac[10] = x_air[6] / x_air[7];
  if (T <= T_L) {
    GetCompleteComb(F, Fc, Ac, x);
  } else if (T < T_H) {
    q = (T - T_L) / (T_H - T_L);
    q = 3.0 * (q * q) - 2.0 * rt_powd_snf(q, 3.0);
    GetCompleteComb(F, Fc, Ac, x);
    GetEquilGrill(P, T, F, Fc, Ac, dv0);
    for (i0 = 0; i0 < 12; i0++) {
      x[i0] = (1.0 - q) * x[i0] + q * dv0[i0];
    }
  } else {
    GetEquilGrill(P, T, F, Fc, Ac, x);
  }
}

/*
 * File trailer for GetCompCombGas.c
 *
 * [EOF]
 */
