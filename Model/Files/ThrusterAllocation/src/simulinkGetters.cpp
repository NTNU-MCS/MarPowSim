//
//  simulinGetters.c
//  
//
//  Created by Torstein I. B?? on 17/03/14.
//
//
#include "simulinkGetters.h"
#include <vector>



vector<double> getDoubleVectorParam(const SimStruct *S, const unsigned int&i,
                                    const unsigned int&length) {
	vector<double> x(length);
	const double* y = getDoubleMatrixParam(S, i);
	for (unsigned int j = 0; j < length; j++)
		x[j] = y[j];
    
	return x;
    
}

vector<unsigned int> getUintVectorParamCast(const SimStruct *S, const unsigned int&i,
                           const unsigned int&length) {
	vector < unsigned int> x(length);
	const double* y = getDoubleMatrixParam(S, i);
	for (unsigned int j = 0; j < length; j++)
		x[j] = (unsigned int) y[j];
    
	return x;
    
}





void getDoubleInputArray(const SimStruct *S, const unsigned int i,
                                const unsigned int length, double* out) {
	InputRealPtrsType x = ssGetInputPortRealSignalPtrs(S, i);
	for (unsigned int j = 0; j < length; j++)
		out[j] = *(x[j]);
}

vector<double> getDoubleVectorInput(const SimStruct *S, const unsigned int i,
                                           const unsigned int&length) {
	vector<double> x(length);
	InputRealPtrsType y = ssGetInputPortRealSignalPtrs(S, i);
	for (unsigned int j = 0; j < length; j++)
		x[j] = *y[j];
    
	return x;
    
}

void getBoolVectorInput(const SimStruct *S, const unsigned int i,
                               const unsigned int length, bool* value) {
	InputBooleanPtrsType y = (InputBooleanPtrsType) ssGetInputPortSignalPtrs(S,
                                                                             i);
	for (unsigned int j = 0; j < length; j++)
		value[j] = *(y[j]);
    
}

vector<unsigned int> getUintVectorInputCast(const SimStruct *S, const unsigned int &i,
                                       const unsigned int &length) {
	vector < unsigned int> x(length);
	InputRealPtrsType y = ssGetInputPortRealSignalPtrs(S, i);
	for (unsigned int j = 0; j < length; j++)
		x[j] = (unsigned int) * (y[j]);
    
	return x;
    
}

vector<double> getDoubleInput(SimStruct *S, unsigned int i, unsigned int length){
    vector <double> x(length);
    if (ssGetInputPortRequiredContiguous(S, i)){
        const double* y = (const double *) ssGetInputPortSignal(S,i);
        for (unsigned int j = 0; j < length; j++)
            x[j] =  y[j];
    } else {
        
	InputRealPtrsType y = ssGetInputPortRealSignalPtrs(S, i);
	for (unsigned int j = 0; j < length; j++)
		x[j] =  * (y[j]);
    }
    
	return x;
}