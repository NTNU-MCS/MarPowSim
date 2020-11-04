/*
 * File: ThdynCV2ZoneConv.c
 *
 *
  *
  *   --- THIS FILE GENERATED BY S-FUNCTION BUILDER: 3.0 ---
  *
  *   This file is an S-function produced by the S-Function
  *   Builder which only recognizes certain fields.  Changes made
  *   outside these fields will be lost the next time the block is
  *   used to load, edit, and resave this file. This file will be overwritten
  *   by the S-function Builder block. If you want to edit this file by hand, 
  *   you must change it only in the area defined as:  
  *
  *        %%%-SFUNWIZ_defines_Changes_BEGIN
  *        #define NAME 'replacement text' 
  *        %%% SFUNWIZ_defines_Changes_END
  *
  *   DO NOT change NAME--Change the 'replacement text' only.
  *
  *   For better compatibility with the Simulink Coder, the
  *   "wrapper" S-function technique is used.  This is discussed
  *   in the Simulink Coder's Manual in the Chapter titled,
  *   "Wrapper S-functions".
  *
  *  -------------------------------------------------------------------------
  * | See matlabroot/simulink/src/sfuntmpl_doc.c for a more detailed template |
  *  ------------------------------------------------------------------------- 
 * Created: Sun Oct  5 23:33:58 2014
 * 
 *
 */

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME ThdynCV2ZoneConv
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
/* %%%-SFUNWIZ_defines_Changes_BEGIN --- EDIT HERE TO _END */
#define NUM_INPUTS          9
/* Input Port  0 */
#define IN_PORT_0_NAME      dmEmb
#define INPUT_0_WIDTH       3
#define INPUT_DIMS_0_COL    1
#define INPUT_0_DTYPE       real_T
#define INPUT_0_COMPLEX     COMPLEX_NO
#define IN_0_FRAME_BASED    FRAME_NO
#define IN_0_BUS_BASED      0
#define IN_0_BUS_NAME       
#define IN_0_DIMS           1-D
#define INPUT_0_FEEDTHROUGH 0
#define IN_0_ISSIGNED        0
#define IN_0_WORDLENGTH      8
#define IN_0_FIXPOINTSCALING 1
#define IN_0_FRACTIONLENGTH  9
#define IN_0_BIAS            0
#define IN_0_SLOPE           0.125
/* Input Port  1 */
#define IN_PORT_1_NAME      dV
#define INPUT_1_WIDTH       1
#define INPUT_DIMS_1_COL    1
#define INPUT_1_DTYPE       real_T
#define INPUT_1_COMPLEX     COMPLEX_NO
#define IN_1_FRAME_BASED    FRAME_NO
#define IN_1_BUS_BASED      0
#define IN_1_BUS_NAME       
#define IN_1_DIMS           1-D
#define INPUT_1_FEEDTHROUGH 0
#define IN_1_ISSIGNED        0
#define IN_1_WORDLENGTH      8
#define IN_1_FIXPOINTSCALING 1
#define IN_1_FRACTIONLENGTH  9
#define IN_1_BIAS            0
#define IN_1_SLOPE           0.125
/* Input Port  2 */
#define IN_PORT_2_NAME      dmfb
#define INPUT_2_WIDTH       1
#define INPUT_DIMS_2_COL    1
#define INPUT_2_DTYPE       real_T
#define INPUT_2_COMPLEX     COMPLEX_NO
#define IN_2_FRAME_BASED    FRAME_NO
#define IN_2_BUS_BASED      0
#define IN_2_BUS_NAME       
#define IN_2_DIMS           1-D
#define INPUT_2_FEEDTHROUGH 0
#define IN_2_ISSIGNED        0
#define IN_2_WORDLENGTH      8
#define IN_2_FIXPOINTSCALING 1
#define IN_2_FRACTIONLENGTH  9
#define IN_2_BIAS            0
#define IN_2_SLOPE           0.125
/* Input Port  3 */
#define IN_PORT_3_NAME      FComb
#define INPUT_3_WIDTH       1
#define INPUT_DIMS_3_COL    1
#define INPUT_3_DTYPE       real_T
#define INPUT_3_COMPLEX     COMPLEX_NO
#define IN_3_FRAME_BASED    FRAME_NO
#define IN_3_BUS_BASED      0
#define IN_3_BUS_NAME       
#define IN_3_DIMS           1-D
#define INPUT_3_FEEDTHROUGH 1
#define IN_3_ISSIGNED        0
#define IN_3_WORDLENGTH      8
#define IN_3_FIXPOINTSCALING 1
#define IN_3_FRACTIONLENGTH  9
#define IN_3_BIAS            0
#define IN_3_SLOPE           0.125
/* Input Port  4 */
#define IN_PORT_4_NAME      dQCyl
#define INPUT_4_WIDTH       1
#define INPUT_DIMS_4_COL    1
#define INPUT_4_DTYPE       real_T
#define INPUT_4_COMPLEX     COMPLEX_NO
#define IN_4_FRAME_BASED    FRAME_NO
#define IN_4_BUS_BASED      0
#define IN_4_BUS_NAME       
#define IN_4_DIMS           1-D
#define INPUT_4_FEEDTHROUGH 0
#define IN_4_ISSIGNED        0
#define IN_4_WORDLENGTH      8
#define IN_4_FIXPOINTSCALING 1
#define IN_4_FRACTIONLENGTH  9
#define IN_4_BIAS            0
#define IN_4_SLOPE           0.125
/* Input Port  5 */
#define IN_PORT_5_NAME      omega
#define INPUT_5_WIDTH       1
#define INPUT_DIMS_5_COL    1
#define INPUT_5_DTYPE       real_T
#define INPUT_5_COMPLEX     COMPLEX_NO
#define IN_5_FRAME_BASED    FRAME_NO
#define IN_5_BUS_BASED      0
#define IN_5_BUS_NAME       
#define IN_5_DIMS           1-D
#define INPUT_5_FEEDTHROUGH 0
#define IN_5_ISSIGNED        0
#define IN_5_WORDLENGTH      8
#define IN_5_FIXPOINTSCALING 1
#define IN_5_FRACTIONLENGTH  9
#define IN_5_BIAS            0
#define IN_5_SLOPE           0.125
/* Input Port  6 */
#define IN_PORT_6_NAME      phi
#define INPUT_6_WIDTH       1
#define INPUT_DIMS_6_COL    1
#define INPUT_6_DTYPE       real_T
#define INPUT_6_COMPLEX     COMPLEX_NO
#define IN_6_FRAME_BASED    FRAME_NO
#define IN_6_BUS_BASED      0
#define IN_6_BUS_NAME       
#define IN_6_DIMS           1-D
#define INPUT_6_FEEDTHROUGH 0
#define IN_6_ISSIGNED        0
#define IN_6_WORDLENGTH      8
#define IN_6_FIXPOINTSCALING 1
#define IN_6_FRACTIONLENGTH  9
#define IN_6_BIAS            0
#define IN_6_SLOPE           0.125
/* Input Port  7 */
#define IN_PORT_7_NAME      combState
#define INPUT_7_WIDTH       1
#define INPUT_DIMS_7_COL    1
#define INPUT_7_DTYPE       real_T
#define INPUT_7_COMPLEX     COMPLEX_NO
#define IN_7_FRAME_BASED    FRAME_NO
#define IN_7_BUS_BASED      0
#define IN_7_BUS_NAME       
#define IN_7_DIMS           1-D
#define INPUT_7_FEEDTHROUGH 0
#define IN_7_ISSIGNED        0
#define IN_7_WORDLENGTH      8
#define IN_7_FIXPOINTSCALING 1
#define IN_7_FRACTIONLENGTH  9
#define IN_7_BIAS            0
#define IN_7_SLOPE           0.125
/* Input Port  8 */
#define IN_PORT_8_NAME      mqf0
#define INPUT_8_WIDTH       1
#define INPUT_DIMS_8_COL    1
#define INPUT_8_DTYPE       real_T
#define INPUT_8_COMPLEX     COMPLEX_NO
#define IN_8_FRAME_BASED    FRAME_NO
#define IN_8_BUS_BASED      0
#define IN_8_BUS_NAME       
#define IN_8_DIMS           1-D
#define INPUT_8_FEEDTHROUGH 0
#define IN_8_ISSIGNED        0
#define IN_8_WORDLENGTH      8
#define IN_8_FIXPOINTSCALING 1
#define IN_8_FRACTIONLENGTH  9
#define IN_8_BIAS            0
#define IN_8_SLOPE           0.125

#define NUM_OUTPUTS          7
/* Output Port  0 */
#define OUT_PORT_0_NAME      p
#define OUTPUT_0_WIDTH       1
#define OUTPUT_DIMS_0_COL    1
#define OUTPUT_0_DTYPE       real_T
#define OUTPUT_0_COMPLEX     COMPLEX_NO
#define OUT_0_FRAME_BASED    FRAME_NO
#define OUT_0_BUS_BASED      0
#define OUT_0_BUS_NAME       
#define OUT_0_DIMS           1-D
#define OUT_0_ISSIGNED        1
#define OUT_0_WORDLENGTH      8
#define OUT_0_FIXPOINTSCALING 1
#define OUT_0_FRACTIONLENGTH  3
#define OUT_0_BIAS            0
#define OUT_0_SLOPE           0.125
/* Output Port  1 */
#define OUT_PORT_1_NAME      Tu
#define OUTPUT_1_WIDTH       1
#define OUTPUT_DIMS_1_COL    1
#define OUTPUT_1_DTYPE       real_T
#define OUTPUT_1_COMPLEX     COMPLEX_NO
#define OUT_1_FRAME_BASED    FRAME_NO
#define OUT_1_BUS_BASED      0
#define OUT_1_BUS_NAME       
#define OUT_1_DIMS           1-D
#define OUT_1_ISSIGNED        1
#define OUT_1_WORDLENGTH      8
#define OUT_1_FIXPOINTSCALING 1
#define OUT_1_FRACTIONLENGTH  3
#define OUT_1_BIAS            0
#define OUT_1_SLOPE           0.125
/* Output Port  2 */
#define OUT_PORT_2_NAME      Fu
#define OUTPUT_2_WIDTH       1
#define OUTPUT_DIMS_2_COL    1
#define OUTPUT_2_DTYPE       real_T
#define OUTPUT_2_COMPLEX     COMPLEX_NO
#define OUT_2_FRAME_BASED    FRAME_NO
#define OUT_2_BUS_BASED      0
#define OUT_2_BUS_NAME       
#define OUT_2_DIMS           1-D
#define OUT_2_ISSIGNED        1
#define OUT_2_WORDLENGTH      8
#define OUT_2_FIXPOINTSCALING 1
#define OUT_2_FRACTIONLENGTH  3
#define OUT_2_BIAS            0
#define OUT_2_SLOPE           0.125
/* Output Port  3 */
#define OUT_PORT_3_NAME      Vu
#define OUTPUT_3_WIDTH       1
#define OUTPUT_DIMS_3_COL    1
#define OUTPUT_3_DTYPE       real_T
#define OUTPUT_3_COMPLEX     COMPLEX_NO
#define OUT_3_FRAME_BASED    FRAME_NO
#define OUT_3_BUS_BASED      0
#define OUT_3_BUS_NAME       
#define OUT_3_DIMS           1-D
#define OUT_3_ISSIGNED        1
#define OUT_3_WORDLENGTH      8
#define OUT_3_FIXPOINTSCALING 1
#define OUT_3_FRACTIONLENGTH  3
#define OUT_3_BIAS            0
#define OUT_3_SLOPE           0.125
/* Output Port  4 */
#define OUT_PORT_4_NAME      Tb
#define OUTPUT_4_WIDTH       1
#define OUTPUT_DIMS_4_COL    1
#define OUTPUT_4_DTYPE       real_T
#define OUTPUT_4_COMPLEX     COMPLEX_NO
#define OUT_4_FRAME_BASED    FRAME_NO
#define OUT_4_BUS_BASED      0
#define OUT_4_BUS_NAME       
#define OUT_4_DIMS           1-D
#define OUT_4_ISSIGNED        1
#define OUT_4_WORDLENGTH      8
#define OUT_4_FIXPOINTSCALING 1
#define OUT_4_FRACTIONLENGTH  3
#define OUT_4_BIAS            0
#define OUT_4_SLOPE           0.125
/* Output Port  5 */
#define OUT_PORT_5_NAME      Fb
#define OUTPUT_5_WIDTH       1
#define OUTPUT_DIMS_5_COL    1
#define OUTPUT_5_DTYPE       real_T
#define OUTPUT_5_COMPLEX     COMPLEX_NO
#define OUT_5_FRAME_BASED    FRAME_NO
#define OUT_5_BUS_BASED      0
#define OUT_5_BUS_NAME       
#define OUT_5_DIMS           1-D
#define OUT_5_ISSIGNED        1
#define OUT_5_WORDLENGTH      8
#define OUT_5_FIXPOINTSCALING 1
#define OUT_5_FRACTIONLENGTH  3
#define OUT_5_BIAS            0
#define OUT_5_SLOPE           0.125
/* Output Port  6 */
#define OUT_PORT_6_NAME      Vb
#define OUTPUT_6_WIDTH       1
#define OUTPUT_DIMS_6_COL    1
#define OUTPUT_6_DTYPE       real_T
#define OUTPUT_6_COMPLEX     COMPLEX_NO
#define OUT_6_FRAME_BASED    FRAME_NO
#define OUT_6_BUS_BASED      0
#define OUT_6_BUS_NAME       
#define OUT_6_DIMS           1-D
#define OUT_6_ISSIGNED        1
#define OUT_6_WORDLENGTH      8
#define OUT_6_FIXPOINTSCALING 1
#define OUT_6_FRACTIONLENGTH  3
#define OUT_6_BIAS            0
#define OUT_6_SLOPE           0.125


#define NPARAMS              10
/* Parameter  1 */
#define PARAMETER_0_NAME      fs
#define PARAMETER_0_DTYPE     real_T
#define PARAMETER_0_COMPLEX   COMPLEX_NO
/* Parameter  2 */
#define PARAMETER_1_NAME      DPhiMix
#define PARAMETER_1_DTYPE     real_T
#define PARAMETER_1_COMPLEX   COMPLEX_NO
/* Parameter  3 */
#define PARAMETER_2_NAME      phiMix0
#define PARAMETER_2_DTYPE     real_T
#define PARAMETER_2_COMPLEX   COMPLEX_NO
/* Parameter  4 */
#define PARAMETER_3_NAME      alpha
#define PARAMETER_3_DTYPE     real_T
#define PARAMETER_3_COMPLEX   COMPLEX_NO
/* Parameter  5 */
#define PARAMETER_4_NAME      hn
#define PARAMETER_4_DTYPE     real_T
#define PARAMETER_4_COMPLEX   COMPLEX_NO
/* Parameter  6 */
#define PARAMETER_5_NAME      p0
#define PARAMETER_5_DTYPE     real_T
#define PARAMETER_5_COMPLEX   COMPLEX_NO
/* Parameter  7 */
#define PARAMETER_6_NAME      T0
#define PARAMETER_6_DTYPE     real_T
#define PARAMETER_6_COMPLEX   COMPLEX_NO
/* Parameter  8 */
#define PARAMETER_7_NAME      F0
#define PARAMETER_7_DTYPE     real_T
#define PARAMETER_7_COMPLEX   COMPLEX_NO
/* Parameter  9 */
#define PARAMETER_8_NAME      V0
#define PARAMETER_8_DTYPE     real_T
#define PARAMETER_8_COMPLEX   COMPLEX_NO
/* Parameter  10 */
#define PARAMETER_9_NAME      nStroke
#define PARAMETER_9_DTYPE     real_T
#define PARAMETER_9_COMPLEX   COMPLEX_NO

#define NDWORKS              2
// DWork1
#define DWORK_0_NAME         mb0
#define DWORK_0_WIDTH        1
#define DWORK_0_DTYPE        real_T
#define DWORK_0_COMPLEX      COMPLEX_NO
// DWork1
#define DWORK_1_NAME         ResetMix
#define DWORK_1_WIDTH        1
#define DWORK_1_DTYPE        real_T
#define DWORK_1_COMPLEX      COMPLEX_NO

#define SAMPLE_TIME_0        INHERITED_SAMPLE_TIME
#define NUM_DISC_STATES      0
#define DISC_STATES_IC       [0]
#define NUM_CONT_STATES      7
#define CONT_STATES_IC       [0,0,0,0,0,0,0,0,0]

#define SFUNWIZ_GENERATE_TLC 1
#define SOURCEFILES "__SFB__GetThdynCombGasZach.c__SFB__"
#define PANELINDEX           6
#define USE_SIMSTRUCT        0
#define SHOW_COMPILE_STEPS   0                   
#define CREATE_DEBUG_MEXFILE 0
#define SAVE_CODE_ONLY       0
#define SFUNWIZ_REVISION     3.0
/* %%%-SFUNWIZ_defines_Changes_END --- EDIT HERE TO _BEGIN */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#include "simstruc.h"
#define PARAM_DEF0(S) ssGetSFcnParam(S, 0)
#define PARAM_DEF1(S) ssGetSFcnParam(S, 1)
#define PARAM_DEF2(S) ssGetSFcnParam(S, 2)
#define PARAM_DEF3(S) ssGetSFcnParam(S, 3)
#define PARAM_DEF4(S) ssGetSFcnParam(S, 4)
#define PARAM_DEF5(S) ssGetSFcnParam(S, 5)
#define PARAM_DEF6(S) ssGetSFcnParam(S, 6)
#define PARAM_DEF7(S) ssGetSFcnParam(S, 7)
#define PARAM_DEF8(S) ssGetSFcnParam(S, 8)
#define PARAM_DEF9(S) ssGetSFcnParam(S, 9)

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

extern void ThdynCV2ZoneConv_Outputs_wrapper(
                          real_T *p,
                          real_T *Tu,
                          real_T *Fu,
                          real_T *Vu,
                          real_T *Tb,
                          real_T *Fb,
                          real_T *Vb ,
                          const real_T *xC);
extern void ThdynCV2ZoneConv_Derivatives_wrapper(const real_T *dmEmb,
                          const real_T *dV,
                          const real_T *dmfb,
                          const real_T *FComb,
                          const real_T *dQCyl,
                          const real_T *omega,
                          const real_T *phi,
                          const real_T *combState,
                          const real_T *mqf0,                
                          const real_T *p,
                          const real_T *Tu,
                          const real_T *Fu,
                          const real_T *Vu,
                          const real_T *Tb,
                          const real_T *Fb,
                          const real_T *Vb,
                          real_T *dx ,
                          real_T *xC, 
                          const real_T *mb0,
                          const real_T  *fs,  const int_T  p_width0,
                          const real_T  *DPhiMix,  const int_T  p_width1,
                          const real_T  *phiMix0,  const int_T  p_width2,
                          const real_T  *alpha,  const int_T  p_width3,
                          const real_T  *hn,  const int_T  p_width4,
                          const real_T  *p0,  const int_T  p_width5,
                          const real_T  *T0,  const int_T  p_width6,
                          const real_T  *F0,  const int_T  p_width7,
                           const real_T *V0, const int_T  p_width8,
                           const real_T *nStroke, const int_T  p_width9);
extern void ThdynCV2ZoneConv_Update_wrapper(const real_T *FComb,
                          const real_T *combState,
                          const real_T *mqf0,
                          const real_T *xC,
                          real_T *mb0,
                          real_T *ResetMix,
                          const real_T  *fs,  const int_T  p_width0,
                          const real_T  *alpha,  const int_T  p_width3);
/*====================*
 * S-function methods *
 *====================*/
#define MDL_CHECK_PARAMETERS
 #if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
   /* Function: mdlCheckParameters =============================================
     * Abstract:
     *    Validate our parameters to verify they are okay.
     */
    static void mdlCheckParameters(SimStruct *S)
    {
     int paramIndex  = 0;
     bool validParam = false;
     /* All parameters must match the S-function Builder Dialog */
     

	 {
	  const mxArray *pVal0 = ssGetSFcnParam(S,0);
	  if (!IS_PARAM_DOUBLE(pVal0)) {
	    validParam = true;
	    paramIndex = 0;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal1 = ssGetSFcnParam(S,1);
	  if (!IS_PARAM_DOUBLE(pVal1)) {
	    validParam = true;
	    paramIndex = 1;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal2 = ssGetSFcnParam(S,2);
	  if (!IS_PARAM_DOUBLE(pVal2)) {
	    validParam = true;
	    paramIndex = 2;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal3 = ssGetSFcnParam(S,3);
	  if (!IS_PARAM_DOUBLE(pVal3)) {
	    validParam = true;
	    paramIndex = 3;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal4 = ssGetSFcnParam(S,4);
	  if (!IS_PARAM_DOUBLE(pVal4)) {
	    validParam = true;
	    paramIndex = 4;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal5 = ssGetSFcnParam(S,5);
	  if (!IS_PARAM_DOUBLE(pVal5)) {
	    validParam = true;
	    paramIndex = 5;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal6 = ssGetSFcnParam(S,6);
	  if (!IS_PARAM_DOUBLE(pVal6)) {
	    validParam = true;
	    paramIndex = 6;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal7 = ssGetSFcnParam(S,7);
	  if (!IS_PARAM_DOUBLE(pVal7)) {
	    validParam = true;
	    paramIndex = 7;
	    goto EXIT_POINT;
	  }
	 }

	 {
	  const mxArray *pVal8 = ssGetSFcnParam(S,8);
	  if (!IS_PARAM_DOUBLE(pVal8)) {
	    validParam = true;
	    paramIndex = 8;
	    goto EXIT_POINT;
	  }
	 }

     {
	  const mxArray *pVal9 = ssGetSFcnParam(S,9);
	  if (!IS_PARAM_DOUBLE(pVal9)) {
	    validParam = true;
	    paramIndex = 9;
	    goto EXIT_POINT;
	  }
	 }
     
     EXIT_POINT:
      if (validParam) {
          char parameterErrorMsg[1024];
          sprintf(parameterErrorMsg, "The data type and or complexity of parameter  %d does not match the "
                  "information specified in the S-function Builder dialog. "
                  "For non-double parameters you will need to cast them using int8, int16, "
                  "int32, uint8, uint16, uint32 or boolean.", paramIndex + 1);
	  ssSetErrorStatus(S,parameterErrorMsg);
      }
	return;
    }
 #endif /* MDL_CHECK_PARAMETERS */
/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *   Setup sizes of the various vectors.
 */
static void mdlInitializeSizes(SimStruct *S)
{

    DECL_AND_INIT_DIMSINFO(inputDimsInfo);
    DECL_AND_INIT_DIMSINFO(outputDimsInfo);
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
      #if defined(MATLAB_MEX_FILE)
	if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
	  mdlCheckParameters(S);
	  if (ssGetErrorStatus(S) != NULL) {
	    return;
	  }
	 } else {
	   return; /* Parameter mismatch will be reported by Simulink */
	 }
      #endif

    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);
    
    ssSetNumContStates(S, NUM_CONT_STATES);
    ssSetNumDiscStates(S, NUM_DISC_STATES);

    if (!ssSetNumInputPorts(S, NUM_INPUTS)) return;
    /*Input Port 0 */
    ssSetInputPortWidth(S,  0, INPUT_0_WIDTH);
    ssSetInputPortDataType(S, 0, SS_DOUBLE);
    ssSetInputPortComplexSignal(S, 0, INPUT_0_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 0, INPUT_0_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 0, 1); /*direct input signal access*/

    /*Input Port 1 */
    ssSetInputPortWidth(S,  1, INPUT_1_WIDTH); /* */
    ssSetInputPortDataType(S, 1, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  1, INPUT_1_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 1, INPUT_1_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 1, 1); /*direct input signal access*/

    /*Input Port 2 */
    ssSetInputPortWidth(S,  2, INPUT_2_WIDTH); /* */
    ssSetInputPortDataType(S, 2, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  2, INPUT_2_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 2, INPUT_2_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 2, 1); /*direct input signal access*/

    /*Input Port 3 */
    ssSetInputPortWidth(S,  3, INPUT_3_WIDTH); /* */
    ssSetInputPortDataType(S, 3, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  3, INPUT_3_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 3, INPUT_3_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 3, 1); /*direct input signal access*/

    /*Input Port 4 */
    ssSetInputPortWidth(S,  4, INPUT_4_WIDTH); /* */
    ssSetInputPortDataType(S, 4, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  4, INPUT_4_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 4, INPUT_4_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 4, 1); /*direct input signal access*/

    /*Input Port 5 */
    ssSetInputPortWidth(S,  5, INPUT_5_WIDTH); /* */
    ssSetInputPortDataType(S, 5, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  5, INPUT_5_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 5, INPUT_5_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 5, 1); /*direct input signal access*/

    /*Input Port 6 */
    ssSetInputPortWidth(S,  6, INPUT_6_WIDTH); /* */
    ssSetInputPortDataType(S, 6, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  6, INPUT_6_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 6, INPUT_6_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 6, 1); /*direct input signal access*/

    /*Input Port 7 */
    ssSetInputPortWidth(S,  7, INPUT_7_WIDTH); /* */
    ssSetInputPortDataType(S, 7, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  7, INPUT_7_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 7, INPUT_7_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 7, 1); /*direct input signal access*/

    /*Input Port 8 */
    ssSetInputPortWidth(S,  8, INPUT_8_WIDTH); /* */
    ssSetInputPortDataType(S, 8, SS_DOUBLE);
    ssSetInputPortComplexSignal(S,  8, INPUT_8_COMPLEX);
    ssSetInputPortDirectFeedThrough(S, 8, INPUT_8_FEEDTHROUGH);
    ssSetInputPortRequiredContiguous(S, 8, 1); /*direct input signal access*/
    

    if (!ssSetNumOutputPorts(S, NUM_OUTPUTS)) return;
    /* Output Port 0 */
    ssSetOutputPortWidth(S, 0, OUTPUT_0_WIDTH);
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 0, OUTPUT_0_COMPLEX);
    /* Output Port 1 */
    ssSetOutputPortWidth(S, 1, OUTPUT_1_WIDTH);
    ssSetOutputPortDataType(S, 1, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 1, OUTPUT_1_COMPLEX);
    /* Output Port 2 */
    ssSetOutputPortWidth(S, 2, OUTPUT_2_WIDTH);
    ssSetOutputPortDataType(S, 2, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 2, OUTPUT_2_COMPLEX);
    /* Output Port 3 */
    ssSetOutputPortWidth(S, 3, OUTPUT_3_WIDTH);
    ssSetOutputPortDataType(S, 3, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 3, OUTPUT_3_COMPLEX);
    /* Output Port 4 */
    ssSetOutputPortWidth(S, 4, OUTPUT_4_WIDTH);
    ssSetOutputPortDataType(S, 4, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 4, OUTPUT_4_COMPLEX);
    /* Output Port 5 */
    ssSetOutputPortWidth(S, 5, OUTPUT_5_WIDTH);
    ssSetOutputPortDataType(S, 5, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 5, OUTPUT_5_COMPLEX);
    /* Output Port 6 */
    ssSetOutputPortWidth(S, 6, OUTPUT_6_WIDTH);
    ssSetOutputPortDataType(S, 6, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 6, OUTPUT_6_COMPLEX);
    
    ssSetNumDWork(S,NDWORKS);
    /*DWork vector 1*/
    ssSetDWorkWidth(S, 0, DWORK_0_WIDTH);
    ssSetDWorkDataType(S, 0, SS_DOUBLE);
    /*DWork vector 2*/
    ssSetDWorkWidth(S, 1, DWORK_1_WIDTH);
    ssSetDWorkDataType(S, 1, SS_DOUBLE);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Take care when specifying exception free code - see sfuntmpl_doc.c */
    ssSetOptions(S, (SS_OPTION_EXCEPTION_FREE_CODE |
                     SS_OPTION_USE_TLC_WITH_ACCELERATOR | 
		     SS_OPTION_WORKS_WITH_CODE_REUSE));
}

#if defined(MATLAB_MEX_FILE)
#define MDL_SET_INPUT_PORT_DIMENSION_INFO
static void mdlSetInputPortDimensionInfo(SimStruct        *S, 
                                         int_T            port,
                                         const DimsInfo_T *dimsInfo)
{
    if(!ssSetInputPortDimensionInfo(S, port, dimsInfo)) return;
}
#endif

#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
#if defined(MDL_SET_OUTPUT_PORT_DIMENSION_INFO)
static void mdlSetOutputPortDimensionInfo(SimStruct        *S, 
                                          int_T            port, 
                                          const DimsInfo_T *dimsInfo)
{
 if (!ssSetOutputPortDimensionInfo(S, port, dimsInfo)) return;
}
#endif
#define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
static void mdlSetDefaultPortDimensionInfo(SimStruct *S)
{
  DECL_AND_INIT_DIMSINFO(portDimsInfo);
  int_T dims[2] = { INPUT_0_WIDTH, 1 };
  bool  frameIn = ssGetInputPortFrameData(S, 0) == FRAME_YES;

  /* Neither the input nor the output ports have been set */

  portDimsInfo.width   = INPUT_0_WIDTH;
  portDimsInfo.numDims = frameIn ? 2 : 1;
  portDimsInfo.dims    = frameIn ? dims : &portDimsInfo.width;
  if (ssGetInputPortNumDimensions(S, 0) == (-1)) {  
   ssSetInputPortDimensionInfo(S, 0, &portDimsInfo);
  }
  portDimsInfo.width   = OUTPUT_0_WIDTH;
  dims[0]              = OUTPUT_0_WIDTH;
  dims[1]              = 1;
  portDimsInfo.dims    = frameIn ? dims : &portDimsInfo.width;
 if (ssGetOutputPortNumDimensions(S, 0) == (-1)) {  
  ssSetOutputPortDimensionInfo(S, 0, &portDimsInfo);
 }
  return;
}
# define MDL_SET_INPUT_PORT_FRAME_DATA
static void mdlSetInputPortFrameData(SimStruct  *S, 
                                     int_T      port,
                                     Frame_T    frameData)
{
    ssSetInputPortFrameData(S, port, frameData);
}
/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    Specifiy  the sample time.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, SAMPLE_TIME_0);
    ssSetOffsetTime(S, 0, 0.0);
}
#define MDL_INITIALIZE_CONDITIONS
 /* Function: mdlInitializeConditions ========================================
  * Abstract:
  *    Initialize the states
  */
 static void mdlInitializeConditions(SimStruct *S)
 {
    real_T *xC   = ssGetContStates(S);
    const real_T  *fs = (const real_T *)mxGetData(PARAM_DEF0(S));
    const real_T  *alpha  = (const real_T *)mxGetData(PARAM_DEF3(S));
    const real_T  *p0  = (const real_T *)mxGetData(PARAM_DEF5(S));
    const real_T  *T0  = (const real_T *)mxGetData(PARAM_DEF6(S));
    const real_T  *F0  = (const real_T *)mxGetData(PARAM_DEF7(S));
    const real_T  *V0  = (const real_T *)mxGetData(PARAM_DEF8(S)); 
    
    real_T  *mb0 = (real_T *)ssGetDWork(S,0);
    real_T  *ResetMix = (real_T *)ssGetDWork(S,1);    
   
    xC[0] =  p0[0];
    xC[1] =  T0[0];
    xC[2] =  F0[0];
    xC[3] =  V0[0]*(1-alpha[0]);
    xC[4] =  T0[0];
    xC[5] =  F0[0];
    xC[6] =  V0[0]*alpha[0];
    mb0[0] = 0;
    ResetMix[0] = 1;

 }
#define MDL_SET_INPUT_PORT_DATA_TYPE
static void mdlSetInputPortDataType(SimStruct *S, int port, DTypeId dType)
{
    ssSetInputPortDataType( S, 0, dType);
}
#define MDL_SET_OUTPUT_PORT_DATA_TYPE
static void mdlSetOutputPortDataType(SimStruct *S, int port, DTypeId dType)
{
    ssSetOutputPortDataType(S, 0, dType);
}

#define MDL_SET_DEFAULT_PORT_DATA_TYPES
static void mdlSetDefaultPortDataTypes(SimStruct *S)
{
  ssSetInputPortDataType( S, 0, SS_DOUBLE);
 ssSetOutputPortDataType(S, 0, SS_DOUBLE);
}
/* Function: mdlOutputs =======================================================
 *
*/
static void mdlOutputs(SimStruct *S, int_T tid)
{
    real_T        *p  = (real_T *)ssGetOutputPortRealSignal(S,0);
    real_T        *Tu  = (real_T *)ssGetOutputPortRealSignal(S,1);
    real_T        *Fu  = (real_T *)ssGetOutputPortRealSignal(S,2);
    real_T        *Vu  = (real_T *)ssGetOutputPortRealSignal(S,3);
    real_T        *Tb  = (real_T *)ssGetOutputPortRealSignal(S,4);
    real_T        *Fb  = (real_T *)ssGetOutputPortRealSignal(S,5);
    real_T        *Vb  = (real_T *)ssGetOutputPortRealSignal(S,6);
    const real_T   *xC = ssGetContStates(S);

    ThdynCV2ZoneConv_Outputs_wrapper(p, Tu, Fu, Vu, Tb, Fb, Vb, xC);
}

#define MDL_DERIVATIVES  /* Change to #undef to remove function */
/* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
    const real_T   *dmEmb  = (const real_T*) ssGetInputPortSignal(S,0);
    const real_T   *dV  = (const real_T*) ssGetInputPortSignal(S,1);
    const real_T   *dmfb  = (const real_T*) ssGetInputPortSignal(S,2);
    const real_T   *FComb  = (const real_T*) ssGetInputPortSignal(S,3);
    const real_T   *dQCyl  = (const real_T*) ssGetInputPortSignal(S,4);
    const real_T   *omega  = (const real_T*) ssGetInputPortSignal(S,5);
    const real_T   *phi  = (const real_T*) ssGetInputPortSignal(S,6);
    const real_T   *combState = (const real_T*) ssGetInputPortSignal(S,7);
    const real_T   *mqf0 = (const real_T*) ssGetInputPortSignal(S,8);
    real_T         *dx  = ssGetdX(S);
    real_T         *xC  = ssGetContStates(S);
    const real_T  *mb0 = (const real_T*)ssGetDWork(S,0);
    real_T        *p  = (real_T *) ssGetOutputPortRealSignal(S,0);
    real_T        *Tu  = (real_T *) ssGetOutputPortRealSignal(S,1);
    real_T        *Fu  = (real_T *) ssGetOutputPortRealSignal(S,2);
    real_T        *Vu  = (real_T *) ssGetOutputPortRealSignal(S,3);
    real_T        *Tb  = (real_T *) ssGetOutputPortRealSignal(S,4);
    real_T        *Fb  = (real_T *) ssGetOutputPortRealSignal(S,5);
    real_T        *Vb  = (real_T *) ssGetOutputPortRealSignal(S,6);
    const int_T   p_width0  = mxGetNumberOfElements(PARAM_DEF0(S));
    const int_T   p_width1  = mxGetNumberOfElements(PARAM_DEF1(S));
    const int_T   p_width2  = mxGetNumberOfElements(PARAM_DEF2(S));
    const int_T   p_width3  = mxGetNumberOfElements(PARAM_DEF3(S));
    const int_T   p_width4  = mxGetNumberOfElements(PARAM_DEF4(S));
    const int_T   p_width5  = mxGetNumberOfElements(PARAM_DEF5(S));
    const int_T   p_width6  = mxGetNumberOfElements(PARAM_DEF6(S));
    const int_T   p_width7  = mxGetNumberOfElements(PARAM_DEF7(S));
    const int_T   p_width8  = mxGetNumberOfElements(PARAM_DEF8(S));
    const int_T   p_width9  = mxGetNumberOfElements(PARAM_DEF9(S));
    const real_T  *fs  = (const real_T *)mxGetData(PARAM_DEF0(S));
    const real_T  *DPhiMix  = (const real_T *)mxGetData(PARAM_DEF1(S));
    const real_T  *phiMix0  = (const real_T *)mxGetData(PARAM_DEF2(S));
    const real_T  *alpha  = (const real_T *)mxGetData(PARAM_DEF3(S));
    const real_T  *hn  = (const real_T *)mxGetData(PARAM_DEF4(S));
    const real_T  *p0  = (const real_T *)mxGetData(PARAM_DEF5(S));
    const real_T  *T0  = (const real_T *)mxGetData(PARAM_DEF6(S));
    const real_T  *F0  = (const real_T *)mxGetData(PARAM_DEF7(S));
    const real_T  *V0  = (const real_T *)mxGetData(PARAM_DEF8(S));
    const real_T  *nStroke  = (const real_T *)mxGetData(PARAM_DEF9(S));

    ThdynCV2ZoneConv_Derivatives_wrapper(dmEmb, dV, dmfb, FComb, dQCyl, omega, phi, combState, mqf0, p, Tu, Fu, Vu, Tb, Fb, Vb,dx,  xC, mb0, fs, p_width0, DPhiMix, p_width1, phiMix0, p_width2, alpha, p_width3, hn, p_width4, p0, p_width5, T0, p_width6, F0, p_width7, V0, p_width8, nStroke, p_width9);
}
#define MDL_UPDATE  /* Change to #undef to remove function */
/* Function: mdlUpdate ======================================================
   * Abstract:
   *    This function is called once for every major integration time step.
   *    Discrete states are typically updated here, but this function is useful
   *    for performing any tasks that should only take place once per
   *    integration step.
   */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
    const real_T   *FComb  = (const real_T*) ssGetInputPortSignal(S,3);
    const real_T   *combState = (const real_T*) ssGetInputPortSignal(S,7);
    const real_T   *mqf0 = (const real_T*) ssGetInputPortSignal(S,8);
    real_T  *mb0 = (real_T*)ssGetDWork(S,0);
    real_T *ResetMix = (real_T*)ssGetDWork(S,1);
    const real_T  *xC  = ssGetContStates(S);
    const int_T   p_width0  = mxGetNumberOfElements(PARAM_DEF0(S));
    const real_T  *fs  = (const real_T *)mxGetData(PARAM_DEF0(S));
    const int_T   p_width3  = mxGetNumberOfElements(PARAM_DEF3(S));
    const real_T  *alpha  = (const real_T *)mxGetData(PARAM_DEF3(S));
    ThdynCV2ZoneConv_Update_wrapper(FComb,combState,mqf0,xC,mb0,ResetMix,fs,p_width0,alpha,p_width3);  
  }

      
/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
}
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif


