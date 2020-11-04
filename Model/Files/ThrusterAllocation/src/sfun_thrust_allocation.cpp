/*  sfun_thrust_allocation.cpp
 *
 * Sfunction wrapper for thrust allocation
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include <iostream>
#include <iomanip>      // std::setprecision
#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_thrust_allocation

#define N_DOF 3

#define P_T_SAMPLE 0
#define P_N_THRUSTERS_TOTAL 1
#define P_N_SWITCHBOARD 2
#define P_SINGULARITY_AVOIDANCE_COST_GAIN 3
#define P_SINGULARITY_AVOIDANCE_COST_SMOOTHER 4
#define P_DF_COST 5
#define P_DANGLE_COST 6
#define N_P 7

#define I_TAU_D 0
#define I_POWER_AVAILABLE_BUS 1
#define I_POWER_AVAILABLE_THRUSTER 2
#define I_ALPHA_OLD 3
#define I_SWITCHBOARD_2_BUS 4
#define I_N_BUS 5
#define I_THRUSTER_ON 6
#define I_F_OLD 7
#define I_TAU_MAX 8
#define I_KT2P 9
#define I_THRUSTER_POSITION_X 10
#define I_THRUSTER_POSITION_Y 11
#define I_THRUSTER_2_SWITCHBOARD 12
#define I_F_RAMP_TIME 13
#define I_TURN_TIME 14
#define I_F_MIN 15
#define I_ROTABLE 16
#define I_REVERSE_THRUST_POWER_GAIN 17

#define N_I 18

#define O_F 0
#define O_ALPHA 1
#define O_POWER 2
#define N_O 3

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
#include "simulinkGetters.h"
#include "ThrustAllocation.h"
#include "mex.h"

//inline const int8_t* getInt8Input(SimStruct *S, unsigned int i) {
//	return (int8_T *) ssGetInputPortSignalPtrs(S, i);
//}

// Writes character stream to MATLAB console.
class MatlabConsoleStreamBuf : public std::basic_streambuf<char>
{
protected:
    int_type overflow( int_type ch = traits_type::eof() ) override
    {
        if (!traits_type::eq_int_type(ch, traits_type::eof()))
            return mexPrintf("%c", traits_type::to_char_type(ch)) > 0 ? 0 : traits_type::eof();

        return 0;
    }
};

// Automatically restores previous rdbuf() when goes out of scope.
class RedirectStream
{
public:
    RedirectStream(std::ostream& stream, std::streambuf& new_streambuf)
    :   _stream(stream)
    ,   _old_streambuf(stream.rdbuf())
    {
        _stream.rdbuf(&new_streambuf);
    }

    ~RedirectStream()
    {
        _stream.rdbuf(_old_streambuf);
    }

private:
    std::ostream& _stream;
    std::streambuf * _old_streambuf;
};

/*====================*
 * S-function methods *
 *====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)
/*
 * Check to make sure that each parameter is 1-d and positive
 */
static void mdlCheckParameters(SimStruct *S) {

	const mxArray *pVal0 = ssGetSFcnParam(S,P_T_SAMPLE);

	if (!is_param_double(pVal0)) {
		ssSetErrorStatus(S,
				"T_SAMPLE in Thruster Allocation must be a double scalar");
		return;
	}

	pVal0 = ssGetSFcnParam(S,P_N_THRUSTERS_TOTAL);

	if (!is_param_double(pVal0)) {
		ssSetErrorStatus(S,
				"N_THRUSTERS_TOTAL in Thruster Allocation must be a integer scalar");
		return;
	}
	int nThurstersTotal = (int) mxGetScalar(pVal0);
	if (nThurstersTotal <= 0) {
		ssSetErrorStatus(S,
				"N_THRUSTERS_TOTAL in Thruster Allocation must be positive");
		return;
	}

	pVal0 = ssGetSFcnParam(S,P_N_SWITCHBOARD);
	if (!is_param_double(pVal0)) {
		ssSetErrorStatus(S,
				"N_SWITCHBOARD in Thruster Allocation must be a integer scalar");
		return;
	}
	double tmp = mxGetScalar(pVal0);
	if (tmp <= 0) {
		ssSetErrorStatus(S,
				"N_SWITCHBOARD in Thruster Allocation must be positive");
		return;
	}

	pVal0 = ssGetSFcnParam(S,P_SINGULARITY_AVOIDANCE_COST_GAIN);
	if (!is_param_double(pVal0)) {
		ssSetErrorStatus(S,
				"SINGULARITY_AVOIDANCE_COST_GAIN in Thruster Allocation must be a double scalar");
		return;
	}
	tmp = mxGetScalar(pVal0);
	if (tmp < 0) {
		ssSetErrorStatus(S,
				"SINGULARITY_AVOIDANCE_COST_GAIN in Thruster Allocation must be non-negative");
		return;
	}

	pVal0 = ssGetSFcnParam(S,P_DF_COST);
	if (!is_param_double(pVal0)) {
		ssSetErrorStatus(S,
				"DF_COST in Thruster Allocation must be a double scalar");
		return;
	}
	tmp = mxGetScalar(pVal0);
	if (tmp < 0) {
		ssSetErrorStatus(S,
				"DF_COST in Thruster Allocation must be non-negative");
		return;
	}
    
    pVal0 = ssGetSFcnParam(S,P_DANGLE_COST);
    if (!is_param_double(pVal0)) {
        ssSetErrorStatus(S,
                         "DANGLE_COST in Thruster Allocation must be a double scalar");
        return;
    }
    tmp = mxGetScalar(pVal0);
    if (tmp < 0) {
        ssSetErrorStatus(S,
                         "DANGLE_COST in Thruster Allocation must be non-negative");
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

	ushort nThurstersTotal = getUshortParam(S, P_N_THRUSTERS_TOTAL);
	ushort nSwitchboard = getUshortParam(S, P_N_SWITCHBOARD);

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
	ssSetSFcnParamTunable(S, 0, 0);

	ssSetNumContStates(S, 0);
	ssSetNumDiscStates(S, 0);

	/// Define input ports and set direct feed through to on
	if (!ssSetNumInputPorts(S, N_I))
		return;
	
	for (unsigned int i = 0; i < N_I; i++) {
		switch (i) {
		case I_TAU_D:
			ssSetInputPortWidth(S, i, N_DOF);
			break;
		case I_POWER_AVAILABLE_BUS:
		case I_SWITCHBOARD_2_BUS:
			ssSetInputPortWidth(S, i, nSwitchboard);
			break;
		case I_POWER_AVAILABLE_THRUSTER:
		case I_ALPHA_OLD:
		case I_TURN_TIME:
		case I_F_RAMP_TIME:
		case I_THRUSTER_ON:
		case I_TAU_MAX:
		case I_KT2P:
		case I_THRUSTER_POSITION_X:
		case I_THRUSTER_POSITION_Y:
		case I_THRUSTER_2_SWITCHBOARD:
		case I_F_OLD:
		case I_F_MIN:
		case I_ROTABLE:
		case I_REVERSE_THRUST_POWER_GAIN:
			ssSetInputPortWidth(S, i, nThurstersTotal);
			break;
		case I_N_BUS:
			ssSetInputPortWidth(S, i, 1);
			break;
		default:
			break;
		}
		ssSetInputPortDirectFeedThrough(S, i, 1);
	}

//	ssSetInputPortDataType(S, I_SWITCHBOARD_2_BUS, SS_UINT32);
//	ssSetInputPortDataType(S, I_N_BUS, SS_UINT32);
	//ssSetInputPortDataType(S, I_THRUSTER_ON, SS_INT8);
	//  ssSetInputPortDataType(S, I_THRUSTER_2_SWITCHBOARD, SS_UINT32);

	if (!ssSetNumOutputPorts(S, N_O))
		return;
	ssSetOutputPortWidth(S, O_F, nThurstersTotal);
	ssSetOutputPortWidth(S, O_ALPHA, nThurstersTotal);
	ssSetOutputPortWidth(S, O_POWER, nThurstersTotal);

	ssSetNumSampleTimes(S, 1);
	ssSetNumRWork(S, 0);
	ssSetNumIWork(S, 0);
	ssSetNumPWork(S, 1);
	// reserve element in the pointers vector
	ssSetNumModes(S, 0);
	// to store a C++ object
	ssSetNumNonsampledZCs(S, 0);

	ssSetSimStateCompliance(S, USE_CUSTOM_SIM_STATE);

	ssSetOptions(S, 0);
}

/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S) {
	ssSetSampleTime(S, 0, getDoubleParam(S, P_T_SAMPLE));
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

template<class T> void printVector(char label[],vector<T> vect){
  cout << label << ": ";
    for (uint i = 0; i < vect.size(); i++)
        cout << std::setprecision(10) << vect[i] << ", ";
    cout << endl;
}

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid) {
    MatlabConsoleStreamBuf mybuf;
    RedirectStream redirect(std::cout, mybuf);

	void* pTA = ssGetPWork(S)[0];
	if (pTA == 0x0) {
		const ushort nThurstersTotal = getUshortParam(S, P_N_THRUSTERS_TOTAL);
		const ushort nSwitchboard = getUshortParam(S, P_N_SWITCHBOARD);
		const double singularityAvoidanceCostGain = getDoubleParam(S,
		P_SINGULARITY_AVOIDANCE_COST_GAIN);
		const double singularityAvoidanceCostSmoother = getDoubleParam(S,
		P_SINGULARITY_AVOIDANCE_COST_SMOOTHER);
        const double dAngleCost = getDoubleParam(S,                                                                      P_DANGLE_COST);
        const double dFCost = getDoubleParam(S,                                                                      P_DF_COST);
		vector<double> TauMax = getDoubleInput(S, I_TAU_MAX, nThurstersTotal);
		vector<double> Kt2p = getDoubleInput(S, I_KT2P, nThurstersTotal);
		vector<uint> thruster2switchboard = getUintVectorInputCast(S,
		I_THRUSTER_2_SWITCHBOARD, nThurstersTotal);
		vector<double> xPosition = getDoubleInput(S, I_THRUSTER_POSITION_X,
				nThurstersTotal);
		vector<double> yPosition = getDoubleInput(S, I_THRUSTER_POSITION_Y,
				nThurstersTotal);
		vector<double> fRampTime = getDoubleInput(S, I_F_RAMP_TIME,
				nThurstersTotal);
		vector<double> turnTime = getDoubleInput(S, I_TURN_TIME,
				nThurstersTotal);
		vector<double> fMin = getDoubleInput(S, I_F_MIN, nThurstersTotal);
		vector<double> rotableDouble = getDoubleInput(S, I_ROTABLE,
				nThurstersTotal);
		bool* rotable = new bool[nThurstersTotal];
		for (uint i = 0; i < nThurstersTotal; i++)
			rotable[i] = rotableDouble[i] > 0;
		vector<double> reverseThrustPowerGain = getDoubleInput(S,
		I_REVERSE_THRUST_POWER_GAIN, nThurstersTotal);
		double sampleTime = getDoubleParam(S, P_T_SAMPLE);
		ThrustAllocation* TA =  new ThrustAllocation(nThurstersTotal, nSwitchboard,
				&TauMax[0], &Kt2p[0], &thruster2switchboard[0], &xPosition[0],
				&yPosition[0], &fRampTime[0], &turnTime[0], &fMin[0], rotable,
				&reverseThrustPowerGain[0], sampleTime,
				singularityAvoidanceCostGain, singularityAvoidanceCostSmoother,dAngleCost,dFCost); // store new C++ object in the
		delete[] rotable;
        vector<double> fOld2 = getDoubleInput(S, I_F_OLD, nThurstersTotal);
        vector<double> alphaOld2 = getDoubleInput(S, I_ALPHA_OLD, nThurstersTotal);
        TA->initialize(&fOld2[0],&alphaOld2[0]);
        pTA = (void *) TA;
		ssGetPWork(S)[0] = pTA;
	}

	ThrustAllocation *TA = (ThrustAllocation *) pTA; // retrieve C++ object from
	const uint nThurstersTotal = TA->getNThrusters();
	const uint nSwitchboards = TA->getNSwitchboard();
	double time = ssGetT(S);

	vector<double> tau_d = getDoubleInput(S, I_TAU_D, N_DOF);
	vector<double> powerAvailableBus = getDoubleInput(S, I_POWER_AVAILABLE_BUS,
			nSwitchboards);
	vector<double> powerAvailableThruster = getDoubleInput(S,
	I_POWER_AVAILABLE_THRUSTER, nThurstersTotal);

	const uint nbus(getDoubleInput(S, I_N_BUS, 1)[0]);
	vector<double> switchboard2busDouble = getDoubleInput(S,
	I_SWITCHBOARD_2_BUS, nSwitchboards);
	uint* switchboard2bus = new uint[nSwitchboards];
	for (uint i = 0; i < nSwitchboards; i++) {
		if (switchboard2busDouble[i] < 0 || switchboard2busDouble[i] >= nbus) {
			ssSetErrorStatus(S,
					"Elements of switchboard2bus sent to Thruster Allocation must be from 0 to nBus - 1");
			return;
		}

		switchboard2bus[i] = switchboard2busDouble[i];
	}

	vector<double> thrusterOnDouble = getDoubleInput(S, I_THRUSTER_ON,
			nThurstersTotal);
	bool* thrusterOn = new bool[nThurstersTotal];
	for (uint i = 0; i < nThurstersTotal; i++)
		thrusterOn[i] = thrusterOnDouble[i] > 0;

	vector<double> fOld = getDoubleInput(S, I_F_OLD, nThurstersTotal);
	vector<double> alphaOld = getDoubleInput(S, I_ALPHA_OLD, nThurstersTotal);
    	TA->initialize(&fOld[0],&alphaOld[0]);
	double* u = ssGetOutputPortRealSignal(S, O_F); // the pointers vector and use
	double* alpha = ssGetOutputPortRealSignal(S, O_ALPHA); // the pointers vector and use
	double* power = ssGetOutputPortRealSignal(S, O_POWER); // the pointers vector and use
	//TA->setPrintLevel(ThrustAllocation::PrintLevel::PL_HIGH);
	bool success;
	try
  	{
		success = TA->thrust(&tau_d[0], &powerAvailableBus[0],
			&powerAvailableThruster[0], &switchboard2bus[0], nbus,
			&thrusterOn[0], &fOld[0], &alphaOld[0], u, alpha, power);
	}
	catch (WrongAcadoException e)
	{
		ssSetErrorStatus(S, "You are using an old version of acado");
	}

	delete[] thrusterOn;
	delete[] switchboard2bus;
	if (!success) {
        
	    cout << "Tau_d (" << std::setprecision(10) << tau_d[0] << "," << std::setprecision(10) << tau_d[1] << "," << std::setprecision(10) << tau_d[2] << ")" << endl;
	    printVector("PowerAvailableBus",powerAvailableBus);
	    printVector("powerAvailableThruster",powerAvailableThruster);
	    printVector("switchboard2bus",switchboard2busDouble);
	    printVector("powerAvailableThruster",powerAvailableThruster);
	    cout << "nbus: " << nbus <<endl;
	    printVector("thrusterOn",thrusterOnDouble);
	    printVector("fOld",fOld);
	    printVector("alphaOld",alphaOld);
	    TA->dump();
		char tmp[100];
		sprintf(tmp, "Thruster allocation failed after %f seconds", time);
		cout.flush();
		//ssWarning(S, tmp);
		ssSetErrorStatus(S, tmp);
	}
	UNUSED_ARG(tid);
	// object
}

#ifdef MATLAB_MEX_FILE
/* For now mdlG[S]etSimState are only supported in normal simulation */

/* Define to indicate that this S-Function has the mdlG[S]etSimState mothods */
#define MDL_SIM_STATE

/* Function: mdlGetSimState =====================================================
 * Abstract:
 *
 */
static mxArray* mdlGetSimState(SimStruct* S) {
	return NULL;
//	counter* c = (counter*) ssGetPWork(S)[0];
//	mxArray* outSS = mxCreateDoubleMatrix(1, 1, mxREAL);
//	mxGetPr(outSS)[0] = c->getX();
//	return outSS;
}
/* Function: mdlGetSimState =====================================================
 * Abstract:
 *
 */
static void mdlSetSimState(SimStruct* S, const mxArray* ma) {
//	counter* c = (counter*) ssGetPWork(S)[0];
//	c->setX(mxGetPr(ma)[0]);
}

#endif

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S) {
    MatlabConsoleStreamBuf mybuf;
    RedirectStream redirect(std::cout, mybuf);
    void* p = ssGetPWork(S)[0];
    if (p != 0x0){
        ThrustAllocation *c = (ThrustAllocation *) p; // retrieve and destroy C++
        delete c;                               // object in the termination
        p = 0;
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

