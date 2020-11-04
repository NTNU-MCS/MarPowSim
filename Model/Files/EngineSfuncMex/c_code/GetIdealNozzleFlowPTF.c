/*
 * File: GetIdealNozzleFlowPTF.c
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
#include "rdivide.h"
#include "sqrt.h"
#include "power.h"
#include "ThdynPack_rtwutil.h"

/* Function Declarations */
static void eml_li_find(boolean_T x, int y_data[], int y_size[2]);

/* Function Definitions */

/*
 * Arguments    : boolean_T x
 *                int y_data[]
 *                int y_size[2]
 * Return Type  : void
 */
static void eml_li_find(boolean_T x, int y_data[], int y_size[2])
{
  int k;
  k = 0;
  if (x) {
    k = 1;
  }

  y_size[0] = 1;
  y_size[1] = k;
  if (x) {
    y_data[0] = 1;
  }
}

/*
 * Calculate the mass flow through ideal nozzle(valve)
 *    Input
 *        Cd, A, p_in, p_out, T_in, F_in
 *    Output
 *        m_dot h_dot m_b
 * Arguments    : double Cd
 *                double A
 *                double p_in
 *                double p_out
 *                double T_in
 *                double F_in
 *                double fs
 *                double *m_dot
 *                double *H_dot
 *                double *m_b_dot
 * Return Type  : void
 */
void GetIdealNozzleFlowPTF(double Cd, double A, double p_in, double p_out,
  double T_in, double F_in, double fs, double *m_dot, double *H_dot, double
  *m_b_dot)
{
  double K;
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
  double gamma1;
  double pr;
  double ht;
  double R;
  int tmp_size[2];
  int tmp_data[1];
  int b_tmp_size[2];
  double b_tmp_data[1];
  int c_tmp_size[2];
  int i7;
  double K_data[1];
  int d_tmp_size[2];
  double c_tmp_data[1];
  int y_size[2];
  double y_data[1];
  double dv36[1];
  int loop_ub;
  int pr_size[2];
  int b_pr_size[2];
  double dv37[1];
  GetThdynCombGasZachV1(p_in, T_in, F_in, fs, &R, &ht, &pr, &gamma1, &unusedU2,
                        &unusedU3, &unusedU4, &unusedU5, &unusedU6, &unusedU7,
                        &unusedU8, &unusedU9, &unusedUa, &unusedUb, &unusedUc,
                        &K);
  pr = rdivide(p_out, p_in);
  gamma1 = rdivide(K - 1.0, K);
  if ((pr < rt_powd_snf(rdivide(2.0, K + 1.0), rdivide(1.0, gamma1))) == 0) {
    *m_dot = rdivide(Cd * A * p_in, sqrt(R * T_in)) * rt_powd_snf(pr, 1.0 / K) *
      sqrt(2.0 / gamma1 * (1.0 - rt_powd_snf(pr, gamma1)));
  } else {
    eml_li_find(true, tmp_data, tmp_size);
    eml_li_find(true, tmp_data, b_tmp_size);
    c_tmp_size[0] = 1;
    c_tmp_size[1] = 1;
    i7 = 0;
    while (i7 <= 0) {
      b_tmp_data[0] = 2.0 / (K + 1.0);
      i7 = 1;
    }

    i7 = 0;
    while (i7 <= 0) {
      K_data[0] = (K + 1.0) / (K - 1.0);
      i7 = 1;
    }

    power(b_tmp_data, c_tmp_size, K_data, c_tmp_data, d_tmp_size);
    pr = Cd * A;
    y_size[0] = 1;
    y_size[1] = b_tmp_size[1];
    i7 = 0;
    while (i7 <= 0) {
      y_data[0] = R * T_in;
      i7 = 1;
    }

    b_sqrt(y_data, y_size);
    d_tmp_size[0] = 1;
    d_tmp_size[1] = 1;
    i7 = 0;
    while (i7 <= 0) {
      c_tmp_data[0] *= K;
      i7 = 1;
    }

    b_sqrt(c_tmp_data, d_tmp_size);
    dv36[0] = 0.0;
    loop_ub = tmp_size[1];
    for (i7 = 0; i7 < loop_ub; i7++) {
      dv36[0] = pr * p_in / y_data[i7] * c_tmp_data[i7];
    }

    y_size[0] = 1;
    y_size[1] = 0;
    b_sqrt(y_data, y_size);
    pr_size[0] = 1;
    pr_size[1] = 0;
    power(K_data, pr_size, b_tmp_data, c_tmp_data, d_tmp_size);
    b_pr_size[0] = 1;
    b_pr_size[1] = 0;
    power(K_data, b_pr_size, K_data, b_tmp_data, tmp_size);
    tmp_size[0] = 1;
    tmp_size[1] = 0;
    b_sqrt(b_tmp_data, tmp_size);
    dv37[0] = dv36[0];
    *m_dot = dv37[0];
  }

  *H_dot = *m_dot * ht;
  *m_b_dot = *m_dot * (F_in * fs) / (1.0 + F_in * fs);
}

/*
 * File trailer for GetIdealNozzleFlowPTF.c
 *
 * [EOF]
 */
