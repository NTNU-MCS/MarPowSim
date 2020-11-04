/*
 * File: GetThdynCombGasZachV1.c
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
 * The routine calculates the thermodynamic property of the combustion gas
 * given pressure, temperature and the fuel air equivalent ratio using
 * Zacharias' method
 *
 *  Input
 *    P : Pressure (Pa)
 *    T : Temperature (K)
 *    F : Fuel-air equivalent ratio
 *    fs : Stoichiometric fuel-air ratio
 *  Output
 *    R : Gas constant
 *    h : Specific enthalpy
 *    s : Specific Entropy
 *    u : Specific internal energy
 *    RF : Partial derivative of R w.r.t. F
 *    RP : Partial derivative of R w.r.t. P
 *    RT : Partial derivative of R w.r.t. T
 *    uF : Partial derivative of u w.r.t. F
 *    uP : Partial derivative of u w.r.t. P
 *    uT : Partial derivative of u w.r.t. T
 *    sF : Partial derivative of s w.r.t. F
 *    sP : Partial derivative of s w.r.t. P
 *    sT : Partial derivative of s w.r.t. T
 *    CP : Specific heat at constant pressure
 *    CV : Specific heat at constant volume
 *    K  : Ratio of specific heats
 * Arguments    : double P
 *                double T
 *                double F
 *                double fs
 *                double *R
 *                double *h
 *                double *s
 *                double *u
 *                double *RF
 *                double *RP
 *                double *RT
 *                double *uF
 *                double *uP
 *                double *uT
 *                double *sF
 *                double *sP
 *                double *sT
 *                double *Cp
 *                double *Cv
 *                double *K
 * Return Type  : void
 */
void GetThdynCombGasZachV1(double P, double T, double F, double fs, double *R,
  double *h, double *s, double *u, double *RF, double *RP, double *RT, double
  *uF, double *uP, double *uT, double *sF, double *sP, double *sT, double *Cp,
  double *Cv, double *K)
{
  double PP;
  double TT;
  double V;
  double VF;
  double EP1;
  double EP2;
  double EP3;
  double EP4;
  double EP5;
  double EP6;
  double PAT1;
  double FC2;
  double FC2F;
  double FC3;
  double FC3F;
  double FC4;
  double FC4F;
  double FC5;
  double FC6;
  double FC6F;
  double FC6T;
  double FC7;
  double FC7P;
  double FC7T;
  double z;
  double FC8;
  double FC8F;
  double FC8T;
  double ZZT1;

  /*  FC0 = 1.01972e-5; */
  PP = 1.01972E-5 * P;
  TT = 0.001 * T;
  V = (1.0 - F) / (1.0 + fs * F);
  VF = -(1.0 + fs * V) / (1.0 + fs * F);
  EP1 = 0.131438 - 0.383504 * V;
  EP2 = 0.477182 + 0.185214 * V;
  EP3 = -0.287367 - 0.0694862 * V;
  EP4 = 0.0742561 + 0.0164041 * V;
  EP5 = -0.00916344 - 0.00204537 * V;
  EP6 = 0.000439896 + 0.00010161 * V;
  PAT1 = EP1 + TT * (2.0 * EP2 + TT * (3.0 * EP3 + TT * (4.0 * EP4 + TT * (5.0 *
    EP5 + TT * 6.0 * EP6))));
  FC2 = 0.000277105 - 9.00711E-5 * V;
  FC2F = -9.00711E-5 * VF;
  FC3 = 0.000642217 - 9.8367E-5 * V;
  FC3F = -9.8367E-5 * VF;
  FC4 = 0.008868 - 0.006131 * V;
  FC4F = -0.006131 * VF;
  FC5 = TT * TT;
  FC6 = exp(FC4 / FC5) / TT;
  FC6F = FC4F * FC6 / FC5;
  FC6T = -FC6 / T * (1.0 + 2.0 * FC4 / FC5);
  FC7 = PP / TT;
  FC7P = 1.01972E-5 / TT;
  FC7T = -FC7 / TT * 0.001;
  z = FC4 / FC5;
  FC8 = 1.0 + FC4 / FC5;
  FC8F = FC4F / FC5;
  FC8T = -FC4 * 2.0E-6 * T / (FC5 * FC5);
  FC4F = FC7 * (FC3 - FC2 * FC6) + 1.0;

  /*  Verified */
  ZZT1 = FC7 * (FC2 * FC6 * ((1.0 + z) + 1.0) - FC3) / TT;
  EP3 = ((3.514956 - 0.005026 * V) + TT * (EP1 + TT * (EP2 + TT * (EP3 + TT *
            (EP4 + TT * (EP5 + TT * EP6)))))) - ZZT1 * TT;
  EP1 = ((((3.5566 - 0.0659 * V) * log(T) + ((2.972979 + 0.55314 * V) + TT *
            ((0.0583837 - 0.485001 * V) + TT * ((0.871349 + 0.0698183 * V) + TT *
              ((-0.451556 - 0.0028772 * V) + TT * ((0.109164 - 0.000693663 * V)
    + TT * ((0.0129754 + 9.20231E-5 * V) + TT * (0.000608234 - 2.99679E-6 * V))))))))
          - (-0.805214 + V * (-0.400981 + V * (5.76989 + 0.060056 * V)))) - log
         (PP)) - FC7 * FC2 * FC6 * (1.0 + z);
  EP4 = 8314.7 * (F / 3.0 + 4.773384) / (9.344254 * F + 138.2266);
  EP2 = (2771.5666666666671 - 9.344254 * EP4) / (9.344254 * F + 138.2266);
  *R = EP4 * FC4F;
  *h = EP4 * T * EP3;
  *s = EP4 * EP1;
  *u = *h - *R * T;
  *RF = EP2 * FC4F + EP4 * (FC7 * ((FC3F - FC2F * FC6) - FC2 * FC6F));
  *RP = EP4 * (FC7P * (FC3 - FC2 * FC6));
  *RT = EP4 * (-FC7 * ((FC3 - FC2 * FC6) * 0.001 / TT + FC2 * FC6T));
  *uF = (EP2 * T * EP3 + EP4 * T * ((-0.005026 + TT * (-0.383504 + TT *
            (0.185214 + TT * (-0.0694862 + TT * (0.0164041 + TT * (-0.00204537 +
    TT * 0.00010161)))))) * VF - FC7 * (((FC2 * FC8F * FC6 - FC3F) + FC2 * ((1.0
              + z) + 1.0) * FC6F) + FC2F * ((1.0 + z) + 1.0) * FC6) / TT * TT))
    - *RF * T;
  *uP = EP4 * T * (-ZZT1 * TT / PP * 1.01972E-5) - *RP * T;
  *uT = (EP4 * (EP3 + T * ((PAT1 * 0.001 - ((FC7T * (FC2 * FC6 * ((1.0 + z) +
    1.0) - FC3) / TT + FC7 * (FC2 * FC6T * ((1.0 + z) + 1.0) + FC2 * FC6 * FC8T)
              / TT) - FC7 * (FC2 * FC6 * ((1.0 + z) + 1.0) - FC3) / (TT * TT) *
             0.001) * TT) - ZZT1 * 0.001)) - *RT * T) - *R;
  *sF = EP2 * EP1 + EP4 * (((((-0.0659 * VF * log(T) + (0.55314 + TT *
    (-0.485001 + TT * (0.0698183 + TT * (-0.0028772 + TT * (-0.000693663 + TT *
                                    (9.20231E-5 - 2.99679E-6 * TT)))))) * VF) -
                              (-0.400981 + V * (11.53978 + 0.180168 * V)) * VF)
    - FC7 * FC2F * FC6 * FC8) - FC7 * FC2 * FC6F * FC8) - FC7 * FC2 * FC6 * FC8F);
  *sP = EP4 * (-1.01972E-5 / PP - FC7P * FC2 * FC6 * FC8);
  *sT = EP4 * (((((3.5566 - 0.0659 * V) / T + ((0.0583837 - 0.485001 * V) + TT *
                   ((1.742698 + 0.1396366 * V) + TT * ((-1.354668 - 0.0086316 *
    V) + TT * ((0.436656 - 0.002774652 * V) + TT * ((0.064877 + 0.0004601155 * V)
    + TT * (0.003649404 - 1.798074E-5 * V)))))) * 0.001) - FC7T * FC2 * FC6 *
                 FC8) - FC7 * FC2 * FC6T * FC8) - FC7 * FC2 * FC6 * FC8T);
  *Cp = EP4 * (EP3 + TT * (-FC7 / TT * (FC3 - 4.0 * FC2 * FC6 * (FC8 * FC8 + FC4
    / FC5)) + PAT1));
  *Cv = *uT + *uP * (*R / T + *RT) / (*R / P - *RP);
  *K = *Cp / *Cv;
}

/*
 * The routine calculates the thermodynamic property of the combustion gas
 * given pressure, temperature and the fuel air equivalent ratio using
 * Zacharias' method
 *
 *  Input
 *    P : Pressure (Pa)
 *    T : Temperature (K)
 *    F : Fuel-air equivalent ratio
 *    fs : Stoichiometric fuel-air ratio
 *  Output
 *    R : Gas constant
 *    h : Specific enthalpy
 *    s : Specific Entropy
 *    u : Specific internal energy
 *    RF : Partial derivative of R w.r.t. F
 *    RP : Partial derivative of R w.r.t. P
 *    RT : Partial derivative of R w.r.t. T
 *    uF : Partial derivative of u w.r.t. F
 *    uP : Partial derivative of u w.r.t. P
 *    uT : Partial derivative of u w.r.t. T
 *    sF : Partial derivative of s w.r.t. F
 *    sP : Partial derivative of s w.r.t. P
 *    sT : Partial derivative of s w.r.t. T
 *    CP : Specific heat at constant pressure
 *    CV : Specific heat at constant volume
 *    K  : Ratio of specific heats
 * Arguments    : double P
 *                double T
 *                double F
 *                double fs
 *                double *R
 *                double *h
 *                double *s
 *                double *u
 *                double *RF
 *                double *RP
 *                double *RT
 *                double *uF
 *                double *uP
 *                double *uT
 *                double *sF
 *                double *sP
 *                double *sT
 * Return Type  : void
 */
void b_GetThdynCombGasZachV1(double P, double T, double F, double fs, double *R,
  double *h, double *s, double *u, double *RF, double *RP, double *RT, double
  *uF, double *uP, double *uT, double *sF, double *sP, double *sT)
{
  double PP;
  double TT;
  double V;
  double VF;
  double EP1;
  double EP2;
  double EP3;
  double EP4;
  double EP5;
  double EP6;
  double FC2;
  double FC2F;
  double FC3;
  double FC3F;
  double FC4;
  double FC4F;
  double FC5;
  double FC6;
  double FC6F;
  double FC6T;
  double FC7;
  double FC7P;
  double FC7T;
  double z;
  double FC8;
  double FC8F;
  double FC8T;
  double SS1;
  double FC11;
  double FC11F;

  /*  FC0 = 1.01972e-5; */
  PP = 1.01972E-5 * P;
  TT = 0.001 * T;
  V = (1.0 - F) / (1.0 + fs * F);
  VF = -(1.0 + fs * V) / (1.0 + fs * F);
  EP1 = 0.131438 - 0.383504 * V;
  EP2 = 0.477182 + 0.185214 * V;
  EP3 = -0.287367 - 0.0694862 * V;
  EP4 = 0.0742561 + 0.0164041 * V;
  EP5 = -0.00916344 - 0.00204537 * V;
  EP6 = 0.000439896 + 0.00010161 * V;
  FC2 = 0.000277105 - 9.00711E-5 * V;
  FC2F = -9.00711E-5 * VF;
  FC3 = 0.000642217 - 9.8367E-5 * V;
  FC3F = -9.8367E-5 * VF;
  FC4 = 0.008868 - 0.006131 * V;
  FC4F = -0.006131 * VF;
  FC5 = TT * TT;
  FC6 = exp(FC4 / FC5) / TT;
  FC6F = FC4F * FC6 / FC5;
  FC6T = -FC6 / T * (1.0 + 2.0 * FC4 / FC5);
  FC7 = PP / TT;
  FC7P = 1.01972E-5 / TT;
  FC7T = -FC7 / TT * 0.001;
  z = FC4 / FC5;
  FC8 = 1.0 + FC4 / FC5;
  FC8F = FC4F / FC5;
  FC8T = -FC4 * 2.0E-6 * T / (FC5 * FC5);
  FC4F = FC7 * (FC3 - FC2 * FC6) + 1.0;

  /*  Verified */
  FC4 = FC7 * (FC2 * FC6 * ((1.0 + z) + 1.0) - FC3) / TT;
  FC5 = ((3.514956 - 0.005026 * V) + TT * (EP1 + TT * (EP2 + TT * (EP3 + TT *
            (EP4 + TT * (EP5 + TT * EP6)))))) - FC4 * TT;
  SS1 = ((((3.5566 - 0.0659 * V) * log(T) + ((2.972979 + 0.55314 * V) + TT *
            ((0.0583837 - 0.485001 * V) + TT * ((0.871349 + 0.0698183 * V) + TT *
              ((-0.451556 - 0.0028772 * V) + TT * ((0.109164 - 0.000693663 * V)
    + TT * ((0.0129754 + 9.20231E-5 * V) + TT * (0.000608234 - 2.99679E-6 * V))))))))
          - (-0.805214 + V * (-0.400981 + V * (5.76989 + 0.060056 * V)))) - log
         (PP)) - FC7 * FC2 * FC6 * (1.0 + z);
  FC11 = 8314.7 * (F / 3.0 + 4.773384) / (9.344254 * F + 138.2266);
  FC11F = (2771.5666666666671 - 9.344254 * FC11) / (9.344254 * F + 138.2266);
  *R = FC11 * FC4F;
  *h = FC11 * T * FC5;
  *s = FC11 * SS1;
  *u = *h - *R * T;
  *RF = FC11F * FC4F + FC11 * (FC7 * ((FC3F - FC2F * FC6) - FC2 * FC6F));
  *RP = FC11 * (FC7P * (FC3 - FC2 * FC6));
  *RT = FC11 * (-FC7 * ((FC3 - FC2 * FC6) * 0.001 / TT + FC2 * FC6T));
  *uF = (FC11F * T * FC5 + FC11 * T * ((-0.005026 + TT * (-0.383504 + TT *
            (0.185214 + TT * (-0.0694862 + TT * (0.0164041 + TT * (-0.00204537 +
    TT * 0.00010161)))))) * VF - FC7 * (((FC2 * FC8F * FC6 - FC3F) + FC2 * ((1.0
              + z) + 1.0) * FC6F) + FC2F * ((1.0 + z) + 1.0) * FC6) / TT * TT))
    - *RF * T;
  *uP = FC11 * T * (-FC4 * TT / PP * 1.01972E-5) - *RP * T;
  *uT = (FC11 * (FC5 + T * (((EP1 + TT * (2.0 * EP2 + TT * (3.0 * EP3 + TT *
    (4.0 * EP4 + TT * (5.0 * EP5 + TT * 6.0 * EP6))))) * 0.001 - ((FC7T * (FC2 *
    FC6 * ((1.0 + z) + 1.0) - FC3) / TT + FC7 * (FC2 * FC6T * ((1.0 + z) + 1.0)
    + FC2 * FC6 * FC8T) / TT) - FC7 * (FC2 * FC6 * ((1.0 + z) + 1.0) - FC3) /
             (TT * TT) * 0.001) * TT) - FC4 * 0.001)) - *RT * T) - *R;
  *sF = FC11F * SS1 + FC11 * (((((-0.0659 * VF * log(T) + (0.55314 + TT *
    (-0.485001 + TT * (0.0698183 + TT * (-0.0028772 + TT * (-0.000693663 + TT *
                                       (9.20231E-5 - 2.99679E-6 * TT)))))) * VF)
    - (-0.400981 + V * (11.53978 + 0.180168 * V)) * VF) - FC7 * FC2F * FC6 * FC8)
    - FC7 * FC2 * FC6F * FC8) - FC7 * FC2 * FC6 * FC8F);
  *sP = FC11 * (-1.01972E-5 / PP - FC7P * FC2 * FC6 * FC8);
  *sT = FC11 * (((((3.5566 - 0.0659 * V) / T + ((0.0583837 - 0.485001 * V) + TT *
                    ((1.742698 + 0.1396366 * V) + TT * ((-1.354668 - 0.0086316 *
    V) + TT * ((0.436656 - 0.002774652 * V) + TT * ((0.064877 + 0.0004601155 * V)
    + TT * (0.003649404 - 1.798074E-5 * V)))))) * 0.001) - FC7T * FC2 * FC6 *
                  FC8) - FC7 * FC2 * FC6T * FC8) - FC7 * FC2 * FC6 * FC8T);
}

/*
 * File trailer for GetThdynCombGasZachV1.c
 *
 * [EOF]
 */
