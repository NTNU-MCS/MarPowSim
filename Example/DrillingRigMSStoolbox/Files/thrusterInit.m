% thrusters = thrusterInit(Thrust0)
%
% Set parameters and initialize thrusters
%
% Inputs:
%    Thrust0  -  Desired thrust of the vessel in body-coordinates: Surge,
%                   Sway, Yaw [N, N, Nm].'
%
% Output:
%    thrusters - struct with parameters for thruster
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%


function [thrusters, ThrusterAllocationInverse] = thrusterInit(Thrust0, nThrusters,thruster_pos_x,thruster_pos_y,T_alpha)

%% Propell values:
AT = [  2.5350E-02	1.7820E-01	1.4674E-02	2.8054E-02	-1.6328E-02	-5.3041E-02	6.0605E-04	3.6823E-02	-2.5429E-03	-1.7680E-02	2.7331E-03	2.1436E-02	-2.4782E-03	1.2317E-03	5.0980E-03	7.8076E-03	-3.7816E-03	3.5353E-03	5.3014E-03	2.1940E-03	-2.8306E-03];
BT = [	0.0000E-00	-7.4777E-01	-1.3822E-02	1.0077E-01	-1.1318E-02	4.7186E-02	1.0666E-02	-9.0239E-03	-7.8452E-03	2.3941E-02	8.0787E-03	-1.4942E-04	-3.1925E-03	9.2620E-03	1.5527E-03	-6.5683E-03	-6.1655E-04	5.1033E-03	-6.0263E-04	-8.2244E-03	-6.3789E-04];
AQ = [	2.4645E-03	2.6718E-02	1.6056E-03	6.5822E-03	-2.2497E-03	-7.8062E-03	2.4126E-04	6.1475E-03	-1.6065E-03	-3.3291E-03	1.2311E-03	3.1123E-03	-1.2559E-03	1.3948E-03	8.8397E-04	5.0358E-05	-7.9990E-04	1.3345E-03	1.1928E-03	-1.3556E-04	-7.0825E-04];
BQ = [	0.0000E-00	-1.1081E-01	1.5909E-04	1.6455E-02	-2.0601E-03	8.5343E-03	8.7856E-04	-3.1327E-03	-9.6650E-04	4.3190E-03	1.2453E-03	9.5986E-05	-7.9986E-04	1.5073E-03	2.4595E-04	-1.6918E-03	5.1603E-04	1.1504E-03	-4.7976E-04	-1.4566E-03	2.3280E-04];


%% Thruster values from Smogeli2006
% Diameter [m]
D = 4;
% Nominal thrust coefficient
KT0 = 0.445;
% Nominal torque coefficient
KQ0 = 0.0666;
%Reverse thrust coefficient
KT0r = 0.347;
%Reversetorquecoefficient
KQ0r = 0.0628;
% Rotational inertia [kgm^2]
Is = 25E3;
% Linear friction coefficient [Nms]
K_omega = 720;
% Static friction [Nm]
Qs = 6.2;
% Gear ratio motor:propeller
k_g = 4;
% Rated motor torque [Nm]
% TODO: needs to verify
QN = 78e3;
% Rated motor power [W]
PN = 4000e3;
% Rated motor speed [rps]
nN = 8.2;
% Max motor torque [Nm]
Qmax = 93e3*10;
% Max motor power [W]
Pmax = 4.8e6;
% Motor time constant [s]
Tm = 0.001;
% Shaft speed filter time constant [s]
Tf = 0.01;
% Motor torque filter time constant [s]
Tfq = 0.01;
% Bollard pull thrust [N]
Tbp = 490e3;
% Bollard pull torque [Nm]
Qbp = 295e3;
% Bollard pull prop. power [W]
Pbp = 3800e3;
% Bollard pull shaft speed [rps]
nbp = 2.05;
% Mech. efficiency
eta_mech = 0.95;
% Density of water [kg/m^3]
rho = 1025;
% Shaft speed noise power
wn = 1E-9;
% Motor torque noise power
wQm = 1.0;

%% Scale factor between given values from Smogeli2006 and desired
scale = 5e6/Pmax;
% Assume n, Ct, Cq, friction coef. is constant through scaling
% Diameter [m]
D = D*(scale^(1/5));
% Rotational inertia [kgm^2] TODO: scale
Is = Is*scale;
% % Nominal thrust coefficient
% KT0 = KT0;
% % Nominal torque coefficient
% KQ0 = KQ0;
% %Reverse thrust coefficient
% KT0r = KT0r;
% %Reversetorquecoefficient
% KQ0r = KQ0r;
% Linear friction coefficient [Nms]
K_omega = K_omega*scale;
% Static friction [Nm]
Qs = Qs*scale;
% % Gear ratio motor:propeller
% k_g = k_g;
% Rated motor torque [Nm]
QN = QN*scale;
% Rated motor power [W]
PN = PN*scale;
% % Rated motor speed [rps]
% nN = nN;
% Max motor torque [Nm]
Qmax = Qmax*scale;
% Max motor power [W]
Pmax = Pmax*scale;
% % Motor time constant [s]
% Tm = Tm;
% % Shaft speed filter time constant [s]
% Tf = Tf;
% % Motor torque filter time constant [s]
% Tfq = Tfq;
% Bollard pull thrust [N]
Tbp = Tbp*(scale^(4/5));
% Bollard pull torque [Nm]
Qbp = Qbp*scale;
% Bollard pull prop. power [W]
Pbp = Pbp*scale;
% % Bollard pull shaft speed [rps]
% nbp = nbp;
% % Mech. efficiency
% eta_mech = eta_mech;
% % Density of water [kg/m^3]
% rho = rho;
% % Shaft speed noise power
% wn = wn;
% % Motor torque noise power
% wQm = wQm;




T_KT(nThrusters) = 0;




for i = 1:nThrusters;
    thrusters(i).command_tau = .2; % Time constant low pass filter thrust command (THRUSTER_T)
    thrusters(i).power_available_tau = .01; % Time constant for DP Power Available reduction (TThrusterFLR)
    thrusters(i).power_available_positive_ramp_time = 3; % Minimum time for increasing power available from 0 to rated power [s]
    thrusters(i).rated_power = PN;% % Rated power of thrusters [N] (thrusterRatedPower)
    thrusters(i).rated_thrust = Tbp;% Thruster torque constant (K_TA)
    thrusters(i).ID = i;% Thruster ID
    thrusters(i).positionX = thruster_pos_x(i);
    thrusters(i).positionY = thruster_pos_y(i);
    thrusters(i).positionZ = -25;
    thrusters(i).alpha0 = T_alpha(i);
    thrusters(i).fRampTime = 5; % Time to go from 0 thrust to rated thrust [s]
    thrusters(i).turnTime = 60; % Time to return the thruster 360 degrees [s]
    thrusters(i).rotable = true; % true if thruster is rotable, false if fixed direction
    thrusters(i).fixThrustTime = realmax; % Set time when thrusters are fixed to:
    thrusters(i).fixThrust = 0;
    thrusters(i).alpha_min = -inf;% Minimum azimuth angle [rad] or -inf if rotated freely [rad]
    thrusters(i).alpha_max = inf;% Maxmimum azimuth angle [rad] or inf if rotated freely [rad]
    thrusters(i).Talpha = .01; % Time constant of azimuth angle [rad]
    thrusters(i).u_min = -1;% Minimum thrust [%]
    thrusters(i).radius = D/2;
    thrusters(i).AT = AT;
    thrusters(i).BT = BT;
    thrusters(i).AQ = AQ;
    thrusters(i).BQ = BQ;
    thrusters(i).KT0 = KT0;
    thrusters(i).KQ0 = KQ0;
    thrusters(i).KP0 = 2*pi*thrusters(i).KQ0/(sqrt(rho)*thrusters(i).radius*2*(thrusters(i).KT0^1.5));
    thrusters(i).KT0r = KT0r;
    thrusters(i).KQ0r = KQ0r;
    thrusters(i).KP0r = 2*pi*thrusters(i).KQ0r/(sqrt(rho)*thrusters(i).radius*2*(thrusters(i).KT0r^1.5));
    % Factor for extra power demand when on reverse power
    thrusters(i).reverseThrustPowerFactor = thrusters(i).KP0r/thrusters(i).KP0;
    thrusters(i).inertia = Is; % Rotational inertia
    thrusters(i).K_omega = K_omega; % Linear friction coefficient [Nms]
    thrusters(i).Qfriction = Qs; % Static friction [Nm]
    thrusters(i).Qmax = Qmax; % Max motor torque [Nm]
    thrusters(i).Pmax = Pmax; % Max motor power [W]
    thrusters(i).k_g = k_g; % Gear ratio motor:propeller
    T_KT(i) = thrusters(i).rated_thrust;
    %% Motor controller parameters
    % PID gains
    thrusters(i).motor.Kp = thrusters(i).Qmax*50; %
    thrusters(i).motor.Ki = thrusters(i).Qmax*5; %
    thrusters(i).motor.Kd = thrusters(i).Qmax*5; %
    % Time constant of first order LF to smooth out omega_ref
    thrusters(i).omega_ref_tau = 2;
    % Dirty derivative gain
    thrusters(i).motor.N = .1;  %
    % Back-calculation gain
    thrusters(i).motor.Kb = .001;%
    % Maximum reverse power (needed for zero-crossing) [W]
    thrusters(i).motor.maxReversePower = Pmax*0.05;
    % Time constant of thruster motor [s]
    thrusters(i).motor.T = 0.001;
    
    
    assert(thrusters(i).motor.maxReversePower <= thrusters(i).Pmax);
end

% Initial thrust command
%X Y N
T_TA0 = [cos(T_alpha).'
    sin(T_alpha).'
    (-cos(T_alpha).*thruster_pos_y+sin(T_alpha).*thruster_pos_x).'];
ThrusterAllocationInverse = pinv(T_TA0*diag(T_KT));
T_u0 = ThrusterAllocationInverse*Thrust0;
for i = 1:nThrusters;
    thrusters(i).initial_thrust = T_u0(i);
    thrusters(i).motor.omega0 = thrusters(i).k_g*...
        findDesiredPropellerSpeed(T_u0(i)*thrusters(i).rated_thrust,thrusters(i),rho);
    thrusters(i).motor.torque0 = getPropellerTQ(0,...
        thrusters(i).motor.omega0/thrusters(i).k_g,thrusters(i),rho);
    
end