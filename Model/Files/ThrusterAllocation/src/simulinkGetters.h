//
//  simulinkGetters.h
//  
//
//  Created by Torstein I. B?? on 17/03/14.
//
//

#ifndef _simulinkGetters_h
#define _simulinkGetters_h
#include "simstruc.h"
#include "matrix.h"
#include <vector>
using  std::vector;

inline double  getDoubleParam(SimStruct *S, unsigned int i) {
	return (double) mxGetScalar(ssGetSFcnParam(S, i));
}

inline unsigned short  getUshortParam(const SimStruct *S, unsigned int i) {
	return (ushort_T) mxGetScalar(ssGetSFcnParam(S, i));
}

inline const double * getDoubleMatrixParam(const SimStruct *S, unsigned int i) {
	return (double *) mxGetData(ssGetSFcnParam(S, i));
}

inline const unsigned int* getUintMatrixParam(SimStruct *S, unsigned int i) {
	return (unsigned int*) mxGetData(ssGetSFcnParam(S, i));
}

vector<double> getDoubleInput(SimStruct *S, unsigned int i, unsigned int length);

inline const uint32_T* getUintInput(SimStruct *S, unsigned int i) {
	return (uint32_T *) ssGetInputPortSignal(S, i);
}

inline unsigned short getUshortInput(const SimStruct *S, const unsigned int i) {
	return (ushort_T)(**ssGetInputPortRealSignalPtrs(S, i));
}

inline bool  is_param_double(const mxArray * const pVal) {
	return mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal)
    && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal);
}

inline bool is_param_double_matrix(const mxArray * const pVal, const unsigned int nrow,
                                   const unsigned int ncol) {
	return (mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal)
			&& !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal)
			&& mxGetN(pVal) == ncol && mxGetM(pVal) == nrow);
}
 
inline bool is_param_uint_matrix(const mxArray * const pVal, const unsigned int nrow,
                                 const unsigned int ncol) {
	return (mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal)
			&& !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsUint32(pVal)
			&& mxGetN(pVal) == ncol && mxGetM(pVal) == nrow);
}

vector<double> getDoubleVectorInput(const SimStruct *S, const unsigned int i,
                                           const unsigned int &length);

vector<unsigned int> getUintVectorInputCast(const SimStruct *S, const unsigned int &i,
                                       const unsigned int &length);


#endif
