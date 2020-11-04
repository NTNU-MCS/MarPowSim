/*    Copyright: 	NTNU, Trondheim, Norway
 *    Licensed under GPL-3.0
 *    Created:  	2013-2017 Torstein Aarseth Bø
 *
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_PMS

#include "simstruc.h"
#include "src/PowerManagementSystem.h"
#include <vector>
#include "mex.h"
#include <iostream>

#define N_DOF 3

#define P_T_SAMPLE 0
#define P_REACTION_TIME_POWER_AVAILABLE 1
#define P_RAMP_TIME_POWER_AVAILABLE 2
#define P_FLR_REACTION_TIME 3
#define P_FLR_RAMP_TIME 4
#define P_NSWITCHBOARDS 5
#define P_NTHRUSTERS 6
#define P_NBATTERIES 7
#define P_DP_DESIRED_BIAS 8
#define P_DP_DESIRED_GAIN 9
#define P_ENABLE_FAULT_DETECTION 10
#define N_P 11

#define I_POWER_DESIRED_LOW_PRIORITY 0
#define I_POWER_LOW_PRIORITY 1

#define I_POWER_DESIRED_HIGH_PRIORITY 2
#define I_POWER_HIGH_PRIORITY 3

#define I_POWER_THRUSTERS 4
#define I_THRUSTER_SWB_ID 5
#define I_THRUSTER_RATED_POWER 6
#define I_THRUSTER_CONNECTED 7

#define I_SWB_2_BUS 8
#define I_N_BUS 9

#define I_POWER_GENERATOR_SETS 10
#define I_POWER_AVAILABLE_GENERATOR_SETS 11
#define I_GENERATOR_SETS_CONNECTED 12
#define I_GENERATOR_SETS_RATED_POWER 13
#define I_GENERATOR_SETS_SWB_ID 14

#define I_BATTERIES_POWER_GENERATED 15
#define I_BATTERIES_POWER_AVAILABLE 16
#define I_BATTERIES_SWB_ID 17

#define N_I 18

#define O_POWER_AVAILABLE_LOW_PRIORITY 0
#define O_POWER_AVAILABLE_HIGH_PRIORITY 1
#define O_POWER_AVAILABLE_DP 2
#define O_POWER_AVAILABLE_THRUSTER 3
#define O_POWER_DP 4
#define O_POWER_DESIRED_DP 5
#define N_O 6

#if defined (_MSC_VER) || defined (__APPLE__)
// Windows stuff
#define uint unsigned int
#define ushort unsigned short
#endif

inline bool IS_PARAM_DOUBLE(const mxArray *pVal) {
  return (mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal)
      && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal));
}

inline bool IS_PARAM_DOUBLE_MATRIX(const mxArray *pVal,
    size_t nrow,
    size_t ncol) {
  return (mxIsNumeric(pVal) && !mxIsLogical(pVal) && !mxIsEmpty(pVal)
      && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal)
      && mxGetN(pVal) == ncol && mxGetM(pVal) == nrow);
}

inline ushort getUshortParam(const SimStruct *S,
    int_T i) {
  return (ushort) mxGetScalar(ssGetSFcnParam(S, i));
}

inline double getDoubleParam(const SimStruct *S,
    int_T i) {
  return mxGetScalar(ssGetSFcnParam(S, i));
}

inline double* getDoubleMatrixParam(const SimStruct *S,
    int_T i) {
  return (double *) mxGetData(ssGetSFcnParam(S, i));
}

vector<double> getDoubleVectorParam(const SimStruct *S,
    const uint &i,
    const uint &length) {
  vector<double> x(length);
  double* y = getDoubleMatrixParam(S, i);
  for (uint j = 0; j < length; j++)
    x[j] = y[j];

  return x;

}

vector<uint> getUintVector(const SimStruct *S,
    const uint &i,
    const uint &length) {
  vector< uint > x(length);
  double* y = getDoubleMatrixParam(S, i);
  for (uint j = 0; j < length; j++)
    x[j] = (uint) y[j];

  return x;

}

/*====================*
 * S-function methods *
 *====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)
/*
 * Check to make sure that each parameter is 1-d and positive
 */
static void mdlCheckParameters(SimStruct *S) {
  if (ssGetSFcnParamsCount(S)!=N_P) {
    ssSetErrorStatus(S,
        "PMS require 11 parameters." );
    return;
  }

  const mxArray *pVal0 = ssGetSFcnParam(S,P_T_SAMPLE);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "T_SAMPLE in PMS must be a double scalar");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_REACTION_TIME_POWER_AVAILABLE);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_REACTION_TIME_POWER_AVAILABLE in PMS must be a double scalar");
    return;
  }
  double tmp = mxGetScalar(pVal0);
  if (tmp <= 0) {
    ssSetErrorStatus(S,
        "P_REACTION_TIME_POWER_AVAILABLE in PMS must be positive");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_RAMP_TIME_POWER_AVAILABLE);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_RAMP_TIME_POWER_AVAILABLE in PMS must be a double scalar");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_FLR_REACTION_TIME);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_FLR_REACTION_TIME in PMS must be a double scalar");
    return;
  }
  tmp = mxGetScalar(pVal0);
  if (tmp <= 0) {
    ssSetErrorStatus(S,
        "P_FLR_REACTION_TIME in PMS must be positive");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_FLR_RAMP_TIME);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_FLR_RAMP_TIME in PMS must be a double scalar");
    return;
  }
  tmp = mxGetScalar(pVal0);
  if (tmp <= 0) {
    ssSetErrorStatus(S,
        "P_FLR_RAMP_TIME in PMS must be positive");
    return;
  }
  pVal0 = ssGetSFcnParam(S,P_NSWITCHBOARDS);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_NSWITCHBOARDS in PMS must be a double scalar");
    return;
  }
  tmp = mxGetScalar(pVal0);
  if (tmp <= 0) {
    ssSetErrorStatus(S,
        "P_NSWITCHBOARDS in PMS must be positive");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_NTHRUSTERS);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_NTHRUSTERS in PMS must be a double scalar");
    return;
  }
  tmp = mxGetScalar(pVal0);
  if (tmp <= 0) {
    ssSetErrorStatus(S,
        "P_NTHRUSTERS in PMS must be positive");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_NBATTERIES);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_NBATTERIES in PMS must be a double scalar");
    return;
  }
  tmp = mxGetScalar(pVal0);
  if (tmp < 0) {
    ssSetErrorStatus(S,
        "P_NBATTERIES in PMS must be non-negative");
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_DP_DESIRED_BIAS);
  if (!IS_PARAM_DOUBLE(pVal0) || mxGetScalar(pVal0) < 0 || mxGetScalar(pVal0)>1) {
    ssSetErrorStatus(S,
        "DP power available bias must be a double scalar between 0 and 1.")
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_DP_DESIRED_GAIN);
  if (!IS_PARAM_DOUBLE(pVal0) || mxGetScalar(pVal0) < 0) {
    ssSetErrorStatus(S,
        "DP power available gain must be a non-negative double scalar")
    return;
  }

  pVal0 = ssGetSFcnParam(S,P_ENABLE_FAULT_DETECTION);
  if (!IS_PARAM_DOUBLE(pVal0)) {
    ssSetErrorStatus(S,
        "P_ENABLE_FAULT_DETECTION in PMS must be a double scalar");
    return;
  }
  tmp = mxGetScalar(pVal0);
  if (!(tmp == 0 || tmp == 1)) {
    ssSetErrorStatus(S,
        "P_ENABLE_FAULT_DETECTION in PMS must be 0 or 1");
    return;
  }
}
#endif

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S) {
  /* See sfuntmpl.doc for more details on the macros below */
  ssSetNumSFcnParams(S, N_P);

  /* Number of expected parameters */
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
  uint nSwb = getDoubleParam(S, P_NSWITCHBOARDS);
  uint nThrusters = getDoubleParam(S, P_NTHRUSTERS);
  uint nBatteries = getDoubleParam(S, P_NBATTERIES);
  ssSetSFcnParamTunable(S, 0, 0);

  ssSetNumContStates(S, 0);
  ssSetNumDiscStates(S, 0);

  /// Define input ports and set direct feed through to on
  if (!ssSetNumInputPorts(S, N_I)) return;

  for (uint i = 0; i < N_I; i++) {
    switch (i) {
    case I_POWER_DESIRED_HIGH_PRIORITY:
    case I_POWER_DESIRED_LOW_PRIORITY:
    case I_POWER_HIGH_PRIORITY:
    case I_POWER_LOW_PRIORITY:
    case I_SWB_2_BUS:
      ssSetInputPortWidth(S, i, nSwb);
      break;
    case I_POWER_THRUSTERS:
    case I_THRUSTER_CONNECTED:
    case I_THRUSTER_RATED_POWER:
    case I_THRUSTER_SWB_ID:
      ssSetInputPortWidth(S, i, nThrusters);
      break;
    case I_BATTERIES_POWER_AVAILABLE:
    case I_BATTERIES_POWER_GENERATED:
    case I_BATTERIES_SWB_ID:
      if (nBatteries > 0) ssSetInputPortWidth(S, i, nBatteries);
      else ssSetInputPortWidth(S, i, 1);
      break;
    case I_N_BUS:
      ssSetInputPortWidth(S, i, 1);
      break;
    default:
      ssSetInputPortWidth(S, i, DYNAMICALLY_SIZED);
      break;
    }
    ssSetInputPortDirectFeedThrough(S, i, 1);

    switch (i) {
    case I_GENERATOR_SETS_CONNECTED:
    case I_THRUSTER_CONNECTED:
      ssSetInputPortDataType(S, i, SS_BOOLEAN);
      break;
    default:
      ssSetInputPortDataType(S, i, SS_DOUBLE);
      break;
    }
  }

  if (!ssSetNumOutputPorts(S, N_O)) return;

  for (uint i = 0; i < N_O; i++) {
    switch (i) {
    case O_POWER_AVAILABLE_THRUSTER:
      ssSetOutputPortWidth(S, i, nThrusters);
      break;
    default:
      ssSetOutputPortWidth(S, i, nSwb);
      break;
    }

  }

  ssSetNumSampleTimes(S, 1);
  ssSetNumRWork(S, 0);
  ssSetNumIWork(S, 0);
  ssSetNumPWork(S, 1);
  // reserve element in the pointers vector
  ssSetNumModes(S, 0);
  // to store a C++ object
  ssSetNumNonsampledZCs(S, 0);

  ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

  ssSetOptions(S, 0);
}

#if defined(MATLAB_MEX_FILE)
#define MDL_SET_INPUT_PORT_DIMENSION_INFO
void mdlSetInputPortDimensionInfo(SimStruct *S, int_T port,
    const DimsInfo_T *dimsInfo) {
  if (dimsInfo->numDims == 1)
  ssSetInputPortDimensionInfo(S,port,dimsInfo);
  else {
    char_T tmp[100];
    sprintf(tmp,"Input port %d has more than one dimension",port);
    ssSetErrorStatus(S, tmp);
  }
  return;
}

#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
void mdlSetOutputPortDimensionInfo(SimStruct *S, int_T port,
    const DimsInfo_T *dimsInfo) {
  if (dimsInfo->numDims == 1)
  ssSetOutputPortDimensionInfo(S,port,dimsInfo);
  else {
    char_T tmp[100];
    sprintf(tmp,"Output port %d has more than one dimension",port);
    ssSetErrorStatus(S, tmp);
  }
  return;
}

#define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
void mdlSetDefaultPortDimensionInfo(SimStruct *S) {
  for (uint i = 0; i< N_I; i++) {
    if (i != I_N_BUS && ssGetInputPortWidth(S, i) == DYNAMICALLY_SIZED) {
      ssSetInputPortWidth(S, i, 1);
    }
  }
  for (uint i = 0; i< N_O; i++) {
    if (ssGetOutputPortWidth(S, i) == DYNAMICALLY_SIZED) {
      ssSetOutputPortWidth(S, i, 1);
    }
  }

}
#endif

/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S) {
  ssSetSampleTime(S, 0, mxGetScalar(ssGetSFcnParam(S, 0)));
  ssSetOffsetTime(S, 0, 0.0);
  ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
/* Function: mdlStart =======================================================
 * Abstract:
 *    This function is called once at start of model execution. If you
 *    have states that should be initialized once, this is the place
 *    to do it.
 */

static void mdlStart(SimStruct *S) {
  ssGetPWork(S)[0] = 0x0;
}                                            // pointers vector
#endif /*  MDL_START */

inline ushort getUshortInput(const SimStruct *S,
    const uint i) {
  return (ushort) (**ssGetInputPortRealSignalPtrs(S, i));
}

inline void getDoubleInputArray(const SimStruct *S,
    const uint i,
    const uint length,
    double* out) {
  InputRealPtrsType x = ssGetInputPortRealSignalPtrs(S, i);
  for (uint j = 0; j < length; j++)
    out[j] = *(x[j]);
}

inline vector<double> getDoubleVectorInput(const SimStruct *S,
    const uint i,
    const uint &length) {
  vector<double> x(length);
  InputRealPtrsType y = ssGetInputPortRealSignalPtrs(S, i);
  for (uint j = 0; j < length; j++)
    x[j] = *y[j];

  return x;

}

inline void getBoolVectorInput(const SimStruct *S,
    const uint i,
    const uint length,
    bool* value) {
  InputBooleanPtrsType y = (InputBooleanPtrsType) ssGetInputPortSignalPtrs(S,
      i);
  for (uint j = 0; j < length; j++)
    value[j] = *(y[j]);

}

static vector<uint> getUintVectorInput(const SimStruct *S,
    const uint &i,
    const uint &length) {
  vector< uint > x(length);
  InputRealPtrsType y = ssGetInputPortRealSignalPtrs(S, i);
  for (uint j = 0; j < length; j++)
    x[j] = (uint) *(y[j]);

  return x;

}

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S,
    int_T tid) {
  void * Ppms = ssGetPWork(S)[0];
  uint nGensets = ssGetInputPortWidth(S, I_GENERATOR_SETS_RATED_POWER);
  uint nSwitchboard = getDoubleParam(S, P_NSWITCHBOARDS);
  uint nThrusters = getDoubleParam(S, P_NTHRUSTERS);
  uint nBatteries = getDoubleParam(S, P_NBATTERIES);

  if (Ppms == 0x0) {
    /// Check if size of all input vector are consistent

    if (nThrusters != ssGetInputPortWidth(S, I_THRUSTER_SWB_ID)
        || nThrusters != ssGetInputPortWidth(S, I_THRUSTER_RATED_POWER)
        || nThrusters != ssGetInputPortWidth(S, I_THRUSTER_CONNECTED)) {
      ssSetErrorStatus(S,
          "All inputs port for thrusters must have the same size");
    }

    if (nGensets != ssGetInputPortWidth(S, I_POWER_GENERATOR_SETS)
        || nGensets != ssGetInputPortWidth(S,
        I_POWER_AVAILABLE_GENERATOR_SETS)
        || nGensets != ssGetInputPortWidth(S, I_GENERATOR_SETS_CONNECTED)
        || nGensets != ssGetInputPortWidth(S, I_GENERATOR_SETS_SWB_ID)) {
      ssSetErrorStatus(S,
          "All inputs port for generator sets must have the same size");
    }

    /// Check if size of all output vectors have correct width
    if (nSwitchboard != ssGetOutputPortWidth(S, O_POWER_AVAILABLE_DP)
        || nSwitchboard != ssGetOutputPortWidth(S,
        O_POWER_AVAILABLE_HIGH_PRIORITY)
        || nSwitchboard != ssGetOutputPortWidth(S,
        O_POWER_AVAILABLE_LOW_PRIORITY)) {
      ssSetErrorStatus(S, "All power available for load output port "
          "must have the same length as number of switchboards.");
    }
    if (nThrusters != ssGetOutputPortWidth(S,
    O_POWER_AVAILABLE_THRUSTER)) {
      ssSetErrorStatus(S, "O_POWER_AVAILABLE_THRUSTER output port "
          "must have the same length as number of thruster.");
    }

    vector<double> thrustersRatedPower = getDoubleVectorInput(S,
    I_THRUSTER_RATED_POWER, nThrusters);
    vector<double> thrustersSwbId = getDoubleVectorInput(S,
    I_THRUSTER_SWB_ID, nThrusters);

    vector<double> gensetsRatedPower = getDoubleVectorInput(S,
    I_GENERATOR_SETS_RATED_POWER, nGensets);
    vector<double> gensetsSwbId = getDoubleVectorInput(S,
    I_GENERATOR_SETS_SWB_ID, nGensets);

    vector<double> batteriesSwbId = getDoubleVectorInput(S,
    I_BATTERIES_SWB_ID, nBatteries);

    PowerPlant p;
    uint thrusterId = 0;
    uint gensetsId = 0;
    uint batteryId = 0;
    double dpPowerAvailableBias = getDoubleParam(S, P_DP_DESIRED_BIAS);
    double dpPowerAvailableGain = getDoubleParam(S, P_DP_DESIRED_GAIN);
    for (unsigned int swbId = 0; swbId < nSwitchboard; swbId++) {
      vector<double> ratedPowerGensetSwb(0);
      vector<double> ratedPowerThrustersSwb(0);
      for (; thrusterId < nThrusters; thrusterId++) {
        if (thrustersSwbId[thrusterId] > swbId) break;
        if (thrustersSwbId[thrusterId] < swbId) {
          ssSetErrorStatus(S,
              "Input of thrusterSwbId should be start with the thruster with smallest"
                  " switchboardID and end with the largest switchboardID");
        }
        ratedPowerThrustersSwb.push_back(thrustersRatedPower[thrusterId]);
      }
      for (; gensetsId < nGensets; gensetsId++) {
        if (gensetsSwbId[gensetsId] > swbId) break;
        if (gensetsSwbId[gensetsId] < swbId) {
          ssSetErrorStatus(S,
              "Input of gensetsSwbId should be start with the geneset with smallest"
                  " switchboardID and end with the largest switchboardID");
        }
        ratedPowerGensetSwb.push_back(gensetsRatedPower[gensetsId]);
      }
      uint nBatteriesSwb = 0;
      for (; batteryId < nBatteries; batteryId++) {
        if (batteriesSwbId[batteryId] > swbId) break;
        if (batteriesSwbId[batteryId] < swbId) {
          ssSetErrorStatus(S,
              "Input of batterySwbId should be start with the battery with smallest"
                  " switchboardID and end with the largest switchboardID");
        }
        nBatteriesSwb++;
      }

      Switchboard s(ratedPowerGensetSwb.size(), &ratedPowerGensetSwb[0],
          ratedPowerThrustersSwb.size(), &ratedPowerThrustersSwb[0],
          nBatteriesSwb, dpPowerAvailableBias, dpPowerAvailableGain);
      p.addSwitchboard(s);
    }
    if (gensetsId != nGensets) {
      ssSetErrorStatus(S,
          "Input of gensets should be start with the geneset with smallest"
              " switchboardID and end with the largest switchboardID");
    }
    if (thrusterId != nThrusters) {
      ssSetErrorStatus(S,
          "Input of thruster should be start with the thruster with smallest"
              " switchboardID and end with the largest switchboardID");
    }
    if (batteryId != nBatteries) {
      ssSetErrorStatus(S,
          "Input of batterySwbId should be start with the battery with smallest"
              " switchboardID and end with the largest switchboardID");
    }

    double reactionTimePowerAvailable = getDoubleParam(S,
    P_REACTION_TIME_POWER_AVAILABLE);
    double rampTimePowerAvailable = getDoubleParam(S,
    P_RAMP_TIME_POWER_AVAILABLE);
    double rampTimeFlr = getDoubleParam(S, P_FLR_RAMP_TIME);
    double reactionTimeFlr = getDoubleParam(S, P_FLR_REACTION_TIME);
    bool enableFaultDetection = getDoubleParam(S, P_ENABLE_FAULT_DETECTION);
    PowerManagementSystem* pms = new PowerManagementSystem(p, reactionTimeFlr,
        rampTimePowerAvailable, reactionTimeFlr, rampTimeFlr,enableFaultDetection);
    Ppms = (void *) pms;
    ssGetPWork(S)[0] = Ppms; // store new C++ object in the

  }
  PowerManagementSystem *pms = (PowerManagementSystem *) Ppms; // retrieve C++ object from
  const PowerPlant *p = pms->getPowerPlant();
  const uint nSwitchboards = p->getNumberOfSwitchboards();

  /// Update Power plant
  // Update Switchboard to bus mapping
  vector< uint > s2b = getUintVectorInput(S, I_SWB_2_BUS, nSwitchboards);
  uint nbus = getUshortInput(S, I_N_BUS);

  // Update Loads
  double* powerLP = new double[nSwitchboards];
  double* powerDesiredLP = new double[nSwitchboards];
  double* powerHP = new double[nSwitchboards];
  double* powerDesiredHP = new double[nSwitchboards];
  getDoubleInputArray(S, I_POWER_LOW_PRIORITY, nSwitchboards, powerLP);
  getDoubleInputArray(S,
  I_POWER_DESIRED_LOW_PRIORITY, nSwitchboards, powerDesiredLP);
  getDoubleInputArray(S, I_POWER_HIGH_PRIORITY, nSwitchboards, powerHP);
  getDoubleInputArray(S,
  I_POWER_DESIRED_HIGH_PRIORITY, nSwitchboards, powerDesiredHP);

  // Update generator sets
  bool* connectedGensets = new bool[nGensets];
  double* powerDelivered = new double[nGensets];
  double* powerAvailable = new double[nGensets];
  getBoolVectorInput(S,
  I_GENERATOR_SETS_CONNECTED, nGensets, connectedGensets);
  getDoubleInputArray(S,
  I_POWER_GENERATOR_SETS, nGensets, powerDelivered);
  getDoubleInputArray(S,
  I_POWER_AVAILABLE_GENERATOR_SETS, nGensets, powerAvailable);

  // Update thruster
  double* powerThruster = new double[nThrusters];
  getDoubleInputArray(S, I_POWER_THRUSTERS, nThrusters, powerThruster);

  // Update batteries
  double* powerBatteries = 0;
  double* powerAvailableBatteries = 0;
  if (nBatteries > 0){
    powerBatteries = new double[nBatteries];
    powerAvailableBatteries = new double[nBatteries];
    getDoubleInputArray(S, I_BATTERIES_POWER_GENERATED, nBatteries, powerBatteries);
    getDoubleInputArray(S, I_BATTERIES_POWER_AVAILABLE, nBatteries, powerAvailableBatteries);
  }

  /// Update PMS
  time_T time = ssGetT(S);
  pms->update(powerDelivered, powerAvailable, connectedGensets, powerBatteries,powerAvailableBatteries, powerHP,
      powerDesiredHP, powerLP, powerDesiredLP, nbus, s2b, powerThruster, time);
  delete[] powerThruster;
  delete[] connectedGensets;
  delete[] powerDelivered;
  delete[] powerAvailable;
  delete[] powerLP;
  delete[] powerDesiredLP;
  delete[] powerHP;
  delete[] powerDesiredHP;
  if (nBatteries > 0){
    delete[] powerBatteries;
    delete[] powerAvailableBatteries;
  }

  /// Populate output
  real_T *paLP = ssGetOutputPortRealSignal(S, O_POWER_AVAILABLE_LOW_PRIORITY);
  real_T *paHP = ssGetOutputPortRealSignal(S,
  O_POWER_AVAILABLE_HIGH_PRIORITY);

  for (uint i = 0; i < nSwitchboards; i++) {
    paLP[i] = p->getSwitchboard(i)->getLoadLowPriority()->getPowerAvailable();
    paHP[i] = p->getSwitchboard(i)->getLoadHighPriority()->getPowerAvailable();
    
  }
  real_T *paDP = ssGetOutputPortRealSignal(S, O_POWER_AVAILABLE_DP);
  real_T *pDP = ssGetOutputPortRealSignal(S, O_POWER_DP);
  real_T *pDDP = ssGetOutputPortRealSignal(S, O_POWER_DESIRED_DP);
  for (uint i = 0; i < nbus; i++) {
    const LoadMerger<DPLoad>* l = p->getBus(i)->getLoadDP();
    paDP[i] = l->getPowerAvailable();
    pDP[i] = l->getLoad();
    pDDP[i] = l->getDesiredLoad();
  }
  for (uint i = nbus; i < nSwitchboards; i++) {
    paDP[i] = 0;
    pDP[i] = 0;
    pDDP[i] = 0;
  }

  real_T *paT = ssGetOutputPortRealSignal(S, O_POWER_AVAILABLE_THRUSTER);
  for (uint i = 0; i < nThrusters; i++) {
    paT[i] = p->getThruster(i)->getPowerAvailable();
  }

  UNUSED_ARG(tid);
}

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S) {
  void* p = ssGetPWork(S)[0];
  if (p != 0x0) {
    PowerManagementSystem *c = (PowerManagementSystem *) ssGetPWork(S)[0]; // retrieve and destroy C++
    delete c;  // object in the termination
    p = 0x0;
  }
}                                              // function
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

