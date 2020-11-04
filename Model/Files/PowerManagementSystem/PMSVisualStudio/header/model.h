#ifndef MODEL_h
# define MODEL_h 
#define MAX_WIDTH 100  
typedef struct {
  	double nSwb;                             
  	double nGeneratorSetPerSwb[MAX_WIDTH];
	double ratedPowerGenset[MAX_WIDTH];
    double nThrustersPerSwb[MAX_WIDTH];
	 double ratedPowerThrusters[MAX_WIDTH];
	   double dpPowerAvailableBias;
	 double dpPowerAvailableGain;
    double reactionTimePA;
    double ramptimePA;
    double reactionTimeFLR;
    double rampTimeFLR;
} Parameters;
#endif
