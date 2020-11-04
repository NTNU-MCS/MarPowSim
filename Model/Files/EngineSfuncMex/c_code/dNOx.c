#include "dNOx.h"

void dNOx(real_T T, real_T N[12], real_T V, real_T NOmol, real_T *dNOConc)
{
    real_T c_X[12];
    real_T k1, k2, k3;
    real_T R1, R2, R3;
    real_T cNO;
    int_T i;
    
    for (i = 0; i < 12; i++) {
        c_X[i] = N[i] / V; //molar concentration[kmol/m3]
    }
    if (c_X[6] == 0){
        dNOConc[0] = 0;
    }
    else{
        //From Heywoods
        /*
        k1 = 7.6e7*exp(-38000/T);
        k2 = 6.4e3*T*exp(-3150/T);
        k3 = 4.1e7;
        */
        //From Hanson and Salimian in kmol/m3
        
        k1 = 1.8e11*exp(-38370/T);  
        k2 = 1.8e7*T*exp(-4680/T);
        k3 = 7.1e10*exp(-450/T);
        
        R1 = k1*c_X[1] * c_X[10];
        R2 = k2*c_X[2] * c_X[7];
        R3 = k3*c_X[2] * c_X[4];
        
        cNO = NOmol / (V);            // in kmol/m3
        dNOConc[0] = 2*R1*(1 - pow(cNO/c_X[6],2)) / (1 + cNO/c_X[6]*R1/(R2 + R3 + 1e-20));
    }
}
