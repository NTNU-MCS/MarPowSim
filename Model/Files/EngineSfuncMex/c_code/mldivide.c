/*
 * File: mldivide.c
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
#include "mldivide.h"

/* Function Definitions */

/*
 * Arguments    : const double A[16]
 *                double B[4]
 * Return Type  : void
 */
void mldivide(const double A[16], double B[4])
{
  double b_A[16];
  signed char ipiv[4];
  int i12;
  int j;
  int c;
  int kAcol;
  int ix;
  double temp;
  int k;
  double s;
  int jy;
  int ijA;
  memcpy(&b_A[0], &A[0], sizeof(double) << 4);
  for (i12 = 0; i12 < 4; i12++) {
    ipiv[i12] = (signed char)(1 + i12);
  }

  for (j = 0; j < 3; j++) {
    c = j * 5;
    kAcol = 0;
    ix = c;
    temp = fabs(b_A[c]);
    for (k = 2; k <= 4 - j; k++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > temp) {
        kAcol = k - 1;
        temp = s;
      }
    }

    if (b_A[c + kAcol] != 0.0) {
      if (kAcol != 0) {
        ipiv[j] = (signed char)((j + kAcol) + 1);
        ix = j;
        kAcol += j;
        for (k = 0; k < 4; k++) {
          temp = b_A[ix];
          b_A[ix] = b_A[kAcol];
          b_A[kAcol] = temp;
          ix += 4;
          kAcol += 4;
        }
      }

      i12 = (c - j) + 4;
      for (jy = c + 1; jy + 1 <= i12; jy++) {
        b_A[jy] /= b_A[c];
      }
    }

    kAcol = c;
    jy = c + 4;
    for (k = 1; k <= 3 - j; k++) {
      temp = b_A[jy];
      if (b_A[jy] != 0.0) {
        ix = c + 1;
        i12 = (kAcol - j) + 8;
        for (ijA = 5 + kAcol; ijA + 1 <= i12; ijA++) {
          b_A[ijA] += b_A[ix] * -temp;
          ix++;
        }
      }

      jy += 4;
      kAcol += 4;
    }
  }

  for (kAcol = 0; kAcol < 3; kAcol++) {
    if (ipiv[kAcol] != kAcol + 1) {
      temp = B[kAcol];
      B[kAcol] = B[ipiv[kAcol] - 1];
      B[ipiv[kAcol] - 1] = temp;
    }
  }

  for (k = 0; k < 4; k++) {
    kAcol = k << 2;
    if (B[k] != 0.0) {
      for (jy = k + 1; jy + 1 < 5; jy++) {
        B[jy] -= B[k] * b_A[jy + kAcol];
      }
    }
  }

  for (k = 3; k > -1; k += -1) {
    kAcol = k << 2;
    if (B[k] != 0.0) {
      B[k] /= b_A[k + kAcol];
      for (jy = 0; jy + 1 <= k; jy++) {
        B[jy] -= B[k] * b_A[jy + kAcol];
      }
    }
  }
}

/*
 * File trailer for mldivide.c
 *
 * [EOF]
 */
