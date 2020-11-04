% init()
%
% Set initial parameters for motion.slx
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%
addpath('Files')
load MDGmodels/rig

% Initial position x, y, z, theta, phi, psi
eta0 = [0,0,0,0,0,0]';
% Initial velocity u, v, w, p, q, r
nu0 = [0,0,0,0,0,0]';
vessel = struct('D',D,'G',G,'MA',MA,'MRB',MRB,'eta0',eta0,'nu0',nu0);

% Prefix for log files
savename = 'log';


%% Switchboard parameters
% number of switchboards
nSwitchboard = 3;
% number of gensets
engines = 6;
% number of thrusters
nThrusters = 6;
nBatteries = 0;

% Water density
rho = 1025;

busTieOpens = 200;

% Generator sets connected
connectedGensets = [ones(1,engines-1) zeros(1,1)].';
connectedGensetsAfter = [ones(1,engines-2) zeros(1,2)].';
connectedGensetsSwitchTime = busTieOpens+50;

% Thruster connected
connectedThrusters = [ones(1,nThrusters-0) zeros(1,0)].';
connectedThrustersAfter = [ones(1,nThrusters-1) zeros(1,1)].';
connectedThrustersSwitchTime = connectedGensetsSwitchTime+2;

%% Loads
t = -1500:1:1e4;
highPriorityLoad = zeros(nSwitchboard,length(t));
lowPriorityLoad = zeros(nSwitchboard,length(t));
avoidLowPriorityReduction = 0;

% Draw works
highPulse = [1.6e6;0;1.6e6]; % watts per switchboard
lowPulse = [0;0;0]; % watts per switchboard
lengthLow = 120; % length of low period in seconds
lengthHigh = 60; % length of high period in seconds
offset = 100; % offset in seconds
drawWorkPriority = 1;
l = makePulseTrain( lowPulse, highPulse,lengthLow,lengthHigh,offset,t );
[highPriorityLoad,lowPriorityLoad ] = addLoad( l ,drawWorkPriority,highPriorityLoad,lowPriorityLoad );

powerFactorHighPriorityLoad = .85*ones(nSwitchboard,1);
powerFactorLowPriorityLoad = .85*ones(nSwitchboard,1);
% Static load which should not be reduced, in Watt
powerHighPriorityBiasLoad = 1e6*[1;1;1]*ones(1,length(t));
powerLowPriorityBiasLoad = 1e6*[1;1;1]*ones(1,length(t));
heaveCompensatorGain = 1e6*[1 0 1].';
heaveCompensatorPriority = [1 0 1].'; % 1 if high priority 0 if low.
PowerDesiredHeaveCompensator = 1.5e3*[1 0 1].';
highPriorityLoad = highPriorityLoad + powerHighPriorityBiasLoad;

tmp = highPriorityLoad;
highPriorityLoad = struct('time',[],'signals',struct('dimensions',0,'values',[]));
for i=1:nSwitchboard
    highPriorityLoad(i) = struct('time',t,'signals',struct('dimensions',[1],'values',tmp(i,:).'));
end

% Static load which will be reduced if the available power is too low

lowPriorityLoad = lowPriorityLoad + powerLowPriorityBiasLoad;

tmp = lowPriorityLoad;
lowPriorityLoad = struct('time',[],'signals',struct('dimensions',0,'values',[]));
for i=1:nSwitchboard
    lowPriorityLoad(i) = struct('time',t,'signals',struct('dimensions',[1],'values',tmp(i,:).'));
end

%% Environment
% Current speed in m/s
u_c = [-1;
    0;
    0];
% Wind force in Newton
windForce = D([1 2 6],[1 2 6])*u_c*1;
% Wave
waveFrequency = .3*2*pi;
waveForceAmplitude = 1*waveFrequency^2*.2*(vessel.MA(1,1)+vessel.MA(1,1))*[1;0;0];

%% DP parameters
% reference position
x_ref = [0; 0; 0];


% Initial value of I-term in PID
thrust0 = -vessel.D([1 2 6],[1 2 6])*u_c-windForce;

% Observer and controller
[observer, dp] = initDP(waveFrequency,vessel,thrust0);
useObserver = 1;

for i = 1:engines
    generators(i).Sb  = 9.1e6;      % Rated value of generator
end

%% Initial values of Thrusters
%% Position of thrusters [m] (x,y) 
xSpread = 35; %Distance from co to thrusters in x-direction
ySpread = 27; % Distance from co to thrusters in y-direction (for those not on x-axis)
thrusterPositionX = [-xSpread, -xSpread, 0, 0, xSpread, xSpread].';
thrusterPositionY = [-ySpread, ySpread, -ySpread, ySpread, -ySpread,  ySpread].';
thruster2swb = [1 1 2 2 3 3];
rotable = [1 1 1 1 1 1];

%% Initial Azimuth-angle of thrusters
thrustersInitialAlpha = [180+45
    -45-180
    180
    0
    -45
    45]*pi/180;
[thrusters, ThrusterAllocationInverse] = thrusterInit(thrust0,nThrusters,thrusterPositionX,thrusterPositionY,thrustersInitialAlpha);

%% Initial values of electrical system
% Initial consumed power (not a parameter, only help variable)
initialPowerFactor = .85;
AvrDroop = .01;
p0 = .5;
q0 = p0*sqrt(1-initialPowerFactor^2)/initialPowerFactor;
clear aux1 aux2 aux3;

%% Initial values for bus
voltageNoQLoad = 1;
voltageBus0 = voltageNoQLoad*(1-AvrDroop*q0)*ones(nSwitchboard,1); % Initial voltage on bus
loadAngle0 =0 *ones(nSwitchboard,1); % Initial loadangle on bus

%% Generator sets parameter
EstimateFuelConsumption = 1;
% % Fuel index to torque gain
% ku = 1+Dfw+r;
for i = 1:engines;
    generators(i).ID  = i;          % Generator ID
    generators(i).powerFactor = .85;% Power factor of thruster
    generators(i).w_b = 60*2*pi;    % Rated speed of generator
    generators(i).r   = 0.0019;     % Stator resistance [p.u.]
    generators(i).xs  = 0.480;      % Synchronous reactance [p.u.]
    generators(i).H   = 2*1.2027;   % Intertia time constant [seconds]
    generators(i).D   = 0.025;      % Damping [p.u.]
    generators(i).fixThrottleTime = realmax; % Time when diesel engines throttle is fixed to:
    generators(i).fixThrottle = 0;
    generators(i).wMax = 1.1;       % Maximum frequency before disconnection
    generators(i).wMin = 0.9;       % Minimum frequency before disconnection
    generators(i).wBreakerTime = 0.1;%Time before generator set is disconnected due to frequency error [seconds]
end

%% Governor parameters
% PID gains
Kp = 20*ones(engines,1); %
Ki = 20*ones(engines,1); %
Kd = 20*ones(engines,1); %
% Dirty derivative gain
N = .1*ones(engines,1);  %
% Droop value
Droop = .02*ones(engines,1); %
% No-load frequency [pu]
wNL0 =  1./(1-Droop*.5);
% Maximum fuel rate [pu]
uMax = 1.5*ones(engines,1); %
% Minimum fuel rate [pu]
uMin = 0*ones(engines,1); %
% Maximum value for power available signal [pu]
pMax = 1.1*ones(engines,1);
% Back-calculation gain
Kb = 1*ones(engines,1);%

for i = 1:engines;
    generators(i).governor.Kp       = 20*0.604322;     % PID Gains
    generators(i).governor.Ki       = 20*0.604322;     % PID Gains
    generators(i).governor.Kd       = 20*0.604322;     % PID Gains
    generators(i).governor.N        = .1;     % Dirty derivative gain
    generators(i).governor.droop    = 0.02;    % Governor droop value
    generators(i).governor.Kb       = 1;      % Back-calculation gain
    generators(i).uMin             = 0;      % Minimum fuel rate [pu]
    generators(i).uMax             = 1.5;    % Maximum fuel rate [pu]
    generators(i).pMax             = 1.1;    % Maximum value for power available signal [p.u.]
    generators(i).governor.wNoLoad = 1/(1-generators(i).governor.droop*.5);    % No load frequency    
    generators(i).governor.rampMax = 0.08109741;
    generators(i).governor.deltaU = 0.19882;
end

%% Air dynamics parameter
% Lower limit of air-to-fuel ratio for combustion
AFlow = 14*ones(engines,1);
% Lower limit of air-to-fuel ratio for full combustion
AFhigh = 23*ones(engines,1);
% Air-to-fuel ratio at rated values
AFn = 27*ones(engines,1);
% Minimum required air-to-fuel ratio (used to saturate fuel index)
AFhighc = 23*ones(engines,1);
ma0 = .2*ones(engines,1);
Tin = 7*ones(engines,1);

E = findInitialE(generators,connectedGensets,voltageBus0,p0,q0);
vNL0 = voltageNoQLoad*ones(engines,1); % No reactive load voltage
%% Automatic Voltage Regulator
for i = 1:engines;

    generators(i).avr.maxFieldVoltage   = 20;      % Maximum field voltage
    generators(i).avr.minFieldVoltage   = 0.01;    % Minimum field voltage
    generators(i).avr.kp       = 400;     % PI Gains
    generators(i).avr.ki       = 100;     % PI Gains
    generators(i).avr.T        = 0.01;   % Low pass filter time constant for AVR
    generators(i).avr.droop    = AvrDroop;    % AVR Droop
    generators(i).theta0       = angle(E(i));
end



% Sampling time of Thrust Allocation [s] or 0 for continous
thrustAllocationSamplingTime = 1;

singularityAvoidanceCostGain = .1;
singularityAvoidanceCostSmoother = .0001;
dfCost = 1;
dAngleCost = 20;

% Sampling time of Power Management system [s] or 0 for continous
pmsSamplingTime = .1;
% Ramp time to upload load from zero to 100% load sharing
rampTimeUpload = 30;
PMSenableFaultDetection=1;

%% Power Available
tLowPriorityLoadLPPowerAvailable = .1; % Time constant for Low Priority Power Available reduction
tHighPriorityLoadLPPowerAvailable = .1; % Time constant for High Priority Power Available reduction
tDPLPPowerAvailable = .1; % Time constant for DP Power Available reduction
tThrusterFLR = .01*ones(nThrusters,1); % Time constant for DP Power Available reduction


%% Efficiency coefficients of generator sets [g/h]:
for i = 1:engines;
    % FC = dm0 + dm1*u + dm2*u^2
    generators(i).turbo.dm0   = 15.939/1000;    % zero order fuel consumption
    generators(i).turbo.dm1   = 155.79/1000;    % first order fuel consumption (FC/P)
    generators(i).turbo.dm2   = 25.497/1000;    % second order fuel consumption (FC/P^2)
    generators(i).turbo.FCmax = (generators(i).turbo.dm0+generators(i).turbo.dm1+generators(i).turbo.dm2)*1.5;
    
    % Fuel efficiency, frequency variations gain
    % FC = (...)*(FCdw2*(dw/dt)^2)
    generators(i).turbo.FCdw2 = 6.09*ones(engines,1);
    
    % Power variations
    % FC = (...)*(FCdp1*(dp/dt) + FCdp2*(dp/dt)^2)
    generators(i).turbo.FCdp1 = .5279;
    generators(i).turbo.FCdp2 = 10.471;
end




% Initial pressure in inlet manifold [p.u.]
% pin0 = 1*ones(engines,1);



for i = 1:engines;
% Initial speed of gensets [pu]    
    generators(i).omega0 = generators(i).governor.wNoLoad * (1-generators(i).governor.droop*p0);
% Initial value of fuel index for generator sets    
    generators(i).u0 = (p0+p0.^2*generators(i).r)./generators(i).omega0 + generators(i).D*generators(i).omega0;
    
    generators(i).avr.v0 = abs(E(i))/generators(i).omega0;       % Initial voltage of AVR
    
end

%% Power Management System
% Time before FLR reacts
reactionTimePowerAvaialble = 1;
% Time from 0 to full load when ramping up load after load reduction
rampTimePowerAvaialble = 45;
% Time before FLR reacts
flrRestoreTime = .2;
% Time from 0 to full load when ramping up load after fast load reduction
flrRampTime = 5;

% Excess power available demand for DP in percentage of rated power of
% thrusters
powerDesiredDPBias = .05;

% Power available demand for DP in percentage of used power by thrusters
powerDesiredDPGain = 1.1;



if ~exist('xFinal','var')
    xFinal = [];
end


initializeMarPowSim;