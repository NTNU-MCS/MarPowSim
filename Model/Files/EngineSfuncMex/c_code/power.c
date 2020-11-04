/*
 * File: power.c
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
#include "power.h"
#include "ThdynPack_rtwutil.h"

/* Function Definitions */

/*
 * Arguments    : const double a_data[]
 *                const int a_size[2]
 *                const double b_data[]
 *                double y_data[]
 *                int y_size[2]
 * Return Type  : void
 */
void power(const double a_data[], const int a_size[2], const double b_data[],
           double y_data[], int y_size[2])
{
  signed char iv0[2];
  int k;
  for (k = 0; k < 2; k++) {
    iv0[k] = (signed char)a_size[k];
  }

  y_size[0] = 1;
  y_size[1] = iv0[1];
  k = 0;
  while (k <= iv0[1] - 1) {
    y_data[0] = rt_powd_snf(a_data[0], b_data[0]);
    k = 1;
  }
}

/*
 * File trailer for power.c
 *
 * [EOF]
 */
