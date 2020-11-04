/* Include headers */
#include "NIVERISTAND_API.h"
#include "model.h"
//#include <stdlib.h>
#include <stddef.h>
void* pmsInitWrapper(const double nSwbD,const double nGeneratorSetPerSwbD[],const double ratedPowerGenset[],
    const double nThrustersPerSwbD[],const double ratedPowerThrusters[],double dpPowerAvailableBias, double dpPowerAvailableGain,
    double reactionTimePA, double ramptimePA,double reactionTimeFLR, double rampTimeFLR);


void pmsWrapper(void* obj, const double* generatorsetPowerDelivered,
    const double* generatorsetPowerAvailable,
    const double* generatorsetConnected,
    const double* powerHighPriority,
    const double* powerDesiredHighPriority,
    const double* powerLowPriority,
    const double* powerDesiredLowPriority,
    const double nBusD,
    const double switchboard2BusD[],
    const double* thrusterPower,
    const double time,
    double* powerAvailableLowPriority,
    double* powerAvailableHighPriority,
    double* powerAvailableDP,
    double* powerConsumedDP,
    double* powerDesiredDP,
    double* powerAvailableThrusters);

void pmsDeleteWrapper(void* obj);

/* User defined datatypes and constants */
#define rtDBL	0

/* Pull in the parameters structure from codegen.c */
extern Parameters rtParameter[2];
extern long READSIDE;		  

/* !!!! IMPORTANT !!!!
   Accessing parameters values must be done through rtParameter[READSIDE]
   The macro readParam is defined for you as a simple way to access parameters
   !!!! IMPORTANT !!!! */
#define readParam rtParameter[READSIDE]

/* INPUT: ptr, base address of where value should be set.
   INPUT: subindex, offset into ptr where value should be set.
   INPUT: value, the value to be set
   INPUT: type, the user defined type of the parameter being set, as defined in
  		  rtParamAttributes
   RETURN: status, NI_ERROR on error, NI_OK otherwise */
long USER_SetValueByDataType(void* ptr, int subindex, double value, int type)
{
	switch (type) {
	case rtDBL: 
    	((double *)ptr)[subindex] = (double)value;
    	return NI_OK;
	}
  	return NI_ERROR;
}

/* INPUT: ptr, base address of value to be retrieved.
   INPUT: subindex, offset into ptr where value should be retrieved.
   INPUT: type, the user defined type of the parameter or signal being 
  		  retrieved, as defined in rtParamAttributes or rtSignalAttributes
   RETURN: value of user-defined type cast to a double */
double USER_GetValueByDataType(void* ptr, int subindex, int type)
{
	switch (type) {
	case rtDBL:
		return (double)(((double *)ptr)[subindex]);
  	}
  	{	/* return NaN, ok for vxworks and pharlap */
	  	unsigned long nan[2] = {0xFFFFFFFF, 0xFFFFFFFF};
		return *(double*)nan;
	}
}

/*
// When a model has parameters of the form: "modelname/parameter", these model parameters are considered workspace variables in veristand
// When a model has parameters of the form: "modelname/block/paramter" these model paramters are NOT considered workspace variables in veristand
// A model should only contain either workspace variables or parameters. Never both.
typedef struct {
  int idx;			// not used
  char* paramname;	// name of the parameter, e.g., "Amplitude"
  unsigned int addr;// offset of the parameter in the Parameters struct
  int datatype;		// integer describing a user defined datatype. must have a corresponding entry in GetValueByDataType and SetValueByDataType
  int width;		// size of parameter
  int numofdims; 	// number of dimensions
  int dimListOffset;// offset into dimensions array
  int IsComplex;	// not used
} NI_Parameter;

*/

/* Define parameter attributes */
long ParameterSize DataSection(".NIVS.paramlistsize") = 11;
NI_Parameter rtParamAttribs[] DataSection(".NIVS.paramlist") = {
  	{ 0, "pms/nSwb_PMS", offsetof(Parameters, nSwb), rtDBL, 1, 2, 0, 0},
  	{ 0, "pms/nGeneratorSetPerSwb_PMS", offsetof(Parameters, nGeneratorSetPerSwb), rtDBL, 1, 2, 2, 0},
	{ 0, "pms/ratedPowerGenset_PMS", offsetof(Parameters,ratedPowerGenset), rtDBL,MAX_WIDTH ,2 ,4 , 0},     
    { 0, "pms/nThrustersPerSwb_PMS", offsetof(Parameters,nThrustersPerSwb ), rtDBL,MAX_WIDTH ,2 ,6 , 0},   
	{ 0, "pms/ratedPowerThrusters_PMS", offsetof(Parameters, ratedPowerThrusters), rtDBL,MAX_WIDTH ,2 ,8 , 0},    
	{ 0, "pms/dpPowerAvailableBias_PMS", offsetof(Parameters, dpPowerAvailableBias), rtDBL,1 ,2 ,10 , 0},    
	{ 0, "pms/dpPowerAvailableGain_PMS", offsetof(Parameters, dpPowerAvailableGain), rtDBL,1 ,2 ,12 , 0},    
    { 0, "pms/reactionTimePA_PMS", offsetof(Parameters, reactionTimePA), rtDBL,1 ,2 ,14 , 0},    
    { 0, "pms/ramptimePA_PMS", offsetof(Parameters, ramptimePA), rtDBL,1 ,2 ,16 , 0},    
    { 0, "pms/reactionTimeFLR_PMS", offsetof(Parameters, reactionTimeFLR), rtDBL,1 ,2 ,18 , 0},    
    { 0, "pms/rampTimeFLR_PMS", offsetof(Parameters, rampTimeFLR), rtDBL,1 ,2 ,20 , 0},    			 
};												   
long ParamDimList[] DataSection(".NIVS.paramdimlist") =
{
  1, 1,  	/* nSwb*/                             
  MAX_WIDTH,1, 	/* nGeneratorSetPerSwb[] */
  MAX_WIDTH,1,	/* ratedPowerGenset[] */
  MAX_WIDTH,1,  /* nThrustersPerSwbD[] */
  MAX_WIDTH,1,	 /* ratedPowerThrusters[] */
 1, 1,	   /* dpPowerAvailableBias */
 1, 1,	 /* dpPowerAvailableGain */
    1, 1, /* reactionTimePA */
   1, 1,  /* ramptimePA */
   1, 1,  /* reactionTimeFLR */
   1, 1,  /* rampTimeFLR */
};

/* Initiatlize parameters */
Parameters initParams DataSection(".NIVS.defaultparams") = {3., 	/* nSwb*/                             
  {2.,2.,2.}, 	/* nGeneratorSetPerSwb[] */
  {9100000.,9100000.,9100000.,9100000.,9100000.,9100000.,},	/* ratedPowerGenset[] */
  {2.,2.,2.},  /* nThrustersPerSwbD[] */
  {4.1667e+06,4.1667e+06,4.1667e+06,4.1667e+06,4.1667e+06,4.1667e+06},	 /* ratedPowerThrusters[] */
 .05, 	   /* dpPowerAvailableBias */
 1.1, 	 /* dpPowerAvailableGain */
     1, /* reactionTimePA */
   45.,  /* ramptimePA */
    .2,  /* reactionTimeFLR */
    5,  /* rampTimeFLR */
};


/*
   This data structure is used to retrieve the size, width, and datatype of the default parameters.
      
   ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
    { sizeof(initParams), 0, 0},  The first element in this array uses only the first field in the typedef.  It is used to specify the size of the default parameters structure.
    { sizeof(double), 1, 0 }, Subsequent elements in the array use all 3 fields, they are: the size (num of bytes per element), the width (num of elements) (2x2 array would have 4 elements), and the datatype of each parameter
   };  */
ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
  { sizeof(initParams)},
  { sizeof(double), 1, rtDBL },
  { sizeof(double), MAX_WIDTH, rtDBL },
  { sizeof(double), MAX_WIDTH, rtDBL },
  { sizeof(double), MAX_WIDTH, rtDBL },
  { sizeof(double), MAX_WIDTH, rtDBL },
  { sizeof(double), 1, rtDBL },
{ sizeof(double), 1, rtDBL }, 
{ sizeof(double), 1, rtDBL }, 
{ sizeof(double), 1, rtDBL }, 
{ sizeof(double), 1, rtDBL }, 
{ sizeof(double), 1, rtDBL }, 
};						 


/* Define signal attributes */
long SignalSize DataSection(".NIVS.siglistsize") = 0;
NI_Signal rtSignalAttribs[1];
long SigDimList[1];

/*
typedef struct {
  int	idx;	// not used
  char*	name;	// name of the external IO, e.g., "In1"
  int	TID;	// = 0
  int   type; 	// Ext Input: 0, Ext Output: 1
  int  width; 	// not used
  int	dimX;	// 1st dimension size
  int	dimY; 	// 2nd dimension size
} NI_ExternalIO;
*/


/* Define IO attributes */
long ExtIOSize DataSection(".NIVS.extlistsize") = 17;
long InportSize = 10;
long OutportSize = 7;
NI_ExternalIO rtIOAttribs[] DataSection(".NIVS.extlist") = {
	{ 0, "generatorsetPowerDelivered", 	0, 0, MAX_WIDTH, 1, MAX_WIDTH},
	{ 0, "generatorsetPowerAvailable", 	0, 0, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "generatorsetConnected", 		0, 0, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerHighPriority", 			0, 0, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerDesiredHighPriority", 	0, 0, MAX_WIDTH, 1, MAX_WIDTH}, 	
	{ 0, "powerLowPriority", 			0, 0, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerDesiredLowPriority", 	0, 0, MAX_WIDTH, 1, MAX_WIDTH},  
	{ 0, "nBusD", 						0, 0, 1, 1, 1},
	{ 0, "switchboard2BusD", 			0, 0, MAX_WIDTH, 1, MAX_WIDTH},  
	{ 0, "thrusterPower", 				0, 0, MAX_WIDTH, 1, MAX_WIDTH},  
	{ 0, "powerAvailableLowPriority", 	0, 1, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerAvailableHighPriority", 	0, 1, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerAvailableDP", 			0, 1, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerConsumedDP", 			0, 1, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerDesiredDP", 				0, 1, MAX_WIDTH, 1, MAX_WIDTH}, 
	{ 0, "powerAvailableThrusters", 	0, 1, MAX_WIDTH, 1, MAX_WIDTH},   
	{ 0, "ValidInput", 	0, 1, 1, 1, 1}, 
	{ -1 },
};

/* Model name and build information */
char * ModelName DataSection(".NIVS.compiledmodelname") = "random";
char * build DataSection(".NIVS.builder") = "VeriStand 2.0 Custom DLL";

/* baserate is the rate at which the model runs */
double baserate = .1;

/*
typedef struct {
  int    tid;		// = 0
  double tstep;		
  double offset;
  int priority;
} NI_Task;
*/

NI_Task rtTaskAttribs DataSection(".NIVS.tasklist") = { 0 /* must be 0 */, .01 /* must be equal to baserate */, 0, 0 };

static void* pmsObj_hjgi2uk143vvkyvhbvkv  = 0x0;  
/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
long USER_Initialize() {
	unsigned int i;
	if (pmsObj_hjgi2uk143vvkyvhbvkv == 0) {
	const double nSwbD = readParam.nSwb;
	const double* nGeneratorSetPerSwbD 	= 	readParam.nGeneratorSetPerSwb;
	const double* ratedPowerGenset 		= 	readParam.ratedPowerGenset; 
    const double* nThrustersPerSwbD 	=	readParam.nThrustersPerSwb; 
	const double* ratedPowerThrusters 	= 	readParam.ratedPowerThrusters; 
	double dpPowerAvailableBias = readParam.dpPowerAvailableBias;
	double dpPowerAvailableGain = readParam.dpPowerAvailableGain;
    double reactionTimePA = readParam.reactionTimePA;
	double ramptimePA = readParam.ramptimePA;
	double reactionTimeFLR = readParam.reactionTimeFLR;
	double rampTimeFLR = readParam.rampTimeFLR;					
	if (nSwbD <= 0 || dpPowerAvailableBias < 0 || dpPowerAvailableGain < 0 || dpPowerAvailableGain < 0 || reactionTimePA < 0 || ramptimePA < 0 || reactionTimeFLR <0 || rampTimeFLR <0)
		return NI_ERROR;
	for (i = 0; i < nSwbD; i++){
		if (nGeneratorSetPerSwbD[i] < 0 || nThrustersPerSwbD[i] < 0)
			return NI_ERROR;
	}
		
	
	pmsObj_hjgi2uk143vvkyvhbvkv = pmsInitWrapper(nSwbD,nGeneratorSetPerSwbD,ratedPowerGenset,
    nThrustersPerSwbD, ratedPowerThrusters, dpPowerAvailableBias,  dpPowerAvailableGain,
     reactionTimePA, ramptimePA, reactionTimeFLR, rampTimeFLR);	  
	}
	return NI_OK;
}

/* INPUT: *inData, pointer to inport data at the current timestamp, to be 
  	      consumed by the function
   OUTPUT: *outData, pointer to outport data at current time + baserate, to be
  	       produced by the function
   INPUT: timestamp, current simulation time */
void USER_TakeOneStep(double *inData, double *outData, double timestamp) 
{	
	if (outData){	
	const double* generatorsetPowerDelivered =  &inData[0*MAX_WIDTH]; 
    const double* generatorsetPowerAvailable =	&inData[1*MAX_WIDTH]; 
    const double* generatorsetConnected =		&inData[2*MAX_WIDTH];  
    const double* powerHighPriority =			&inData[3*MAX_WIDTH];  
    const double* powerDesiredHighPriority =	&inData[4*MAX_WIDTH]; 
    const double* powerLowPriority =			&inData[5*MAX_WIDTH]; 
    const double* powerDesiredLowPriority =		&inData[6*MAX_WIDTH]; 
    const double nBusD =						 inData[7*MAX_WIDTH];
    const double* switchboard2BusD =			&inData[7*MAX_WIDTH+1];
    const double* thrusterPower =				&inData[8*MAX_WIDTH+1];
    const double time = timestamp;
    double* powerAvailableLowPriority =	&outData[0*MAX_WIDTH]; 
    double* powerAvailableHighPriority =&outData[1*MAX_WIDTH]; 
    double* powerAvailableDP =			&outData[2*MAX_WIDTH];
    double* powerConsumedDP =			&outData[3*MAX_WIDTH];  
    double* powerDesiredDP =			&outData[4*MAX_WIDTH];  
    double* powerAvailableThrusters =	&outData[5*MAX_WIDTH];   
	double* validInput =				&outData[6*MAX_WIDTH]; 
	unsigned int i;
	
	*validInput = 1;
	if (nBusD <= 0){
		*validInput = 0;
		return;
	}
	
	for (i = 0; i < readParam.nSwb; i++){
		if ( switchboard2BusD[i] < 0 ||  switchboard2BusD[i] >= readParam.nSwb) {
			*validInput = 0;
			return;
		}
	}
		


	pmsWrapper(pmsObj_hjgi2uk143vvkyvhbvkv, generatorsetPowerDelivered,
    generatorsetPowerAvailable,
    generatorsetConnected,
    powerHighPriority,
    powerDesiredHighPriority,
    powerLowPriority,
    powerDesiredLowPriority,
    nBusD,
    switchboard2BusD,
    thrusterPower,
    time,
    powerAvailableLowPriority,
    powerAvailableHighPriority,
    powerAvailableDP,
    powerConsumedDP,
    powerDesiredDP,
    powerAvailableThrusters);
	}
		
}

/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
long USER_Finalize() {
	pmsDeleteWrapper( pmsObj_hjgi2uk143vvkyvhbvkv);
	pmsObj_hjgi2uk143vvkyvhbvkv = 0;
	return NI_OK;
}
