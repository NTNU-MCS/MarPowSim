% [Observer, DP] = initDP(w0,M,D,thrust0)
%
% Set parameters for DP and Observer, for Fossens passive observer
% Observers is tunned by suggested method in Handbook of Guidance and
% Control. DP controller is tuned to have an eigenfrequency at 1/30 rad/s
% and relative damping of 1 (criticaly damped).
%
% Inputs:
%    w0 - peak frequency of wave spectrum, scalar
%    vessel - Struct with vessel parameters
%    thrust0 - Initial thrust of DP [N, N, Nm]
%
% Outputs:
%    Observer - Observer parameters
%    DP - DP parameters
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	2014.08.06	Torstein Aarseth Bø <torstein.bo@sintef.no> Changed input and
%                                                    initialization of observer
%    		<date>	<author> <description>
%
function [Observer, DP] = initDP(w0,vessel,thrust0)
dim = [1 2 6];
M = vessel.MRB(dim,dim)+vessel.MA(dim,dim);   % Total mass
D = vessel.D;
DOF = 3;
w0 = w0*ones(DOF,1);

% Cut-off frequency - radians
wc = w0*1.1;
zeta = .1*ones(DOF,1);
Zetan = 1*ones(DOF,1);

Observer.Aw = [zeros(3,3) eye(3,3);
    -diag(w0)^2 -2*diag(zeta)*diag(w0)];
Observer.Cw = [zeros(3) eye(3)];


% Augmented Observer gains - by tuning rules
Observer.K1 = [diag(-2*(Zetan-zeta).*(wc./w0));
    diag(2*w0.*(Zetan-zeta))];
Observer.K2 = diag(wc);
Observer.K3 = M*1e-3;
Observer.K4 = M*1e-3;
T1 = 1000;
Observer.T = diag([T1 T1 T1]);
Observer.M = M;
Observer.D = D;


% Initialization of Observer
Observer.etaHat0 = vessel.eta0(dim);
Observer.nuHat0 = vessel.nu0(dim);
Observer.biasForce0 = -thrust0;

%% Controller gains
w0Control = diag(1/30*[1 1 1]); % Eigenfrequency of DP
XiControl = diag([1 1 1]);      % Relativ damping of DP
DP.kp = w0Control.^2*M;      % P-gain in DP
DP.kd = (2*XiControl*w0Control*M-D(dim,dim)); % D-gain in DP
DP.ki = DP.kp*1/100;      % I-gain in DP
DP.tau0 = thrust0;
