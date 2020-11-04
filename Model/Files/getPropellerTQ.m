% [ Q,T ] = getPropellerTQ( Va,omega,thruster,rho )
%
% Calculates the thrust and torque from 4-quadrant model
%
% Inputs:
%    Va - apperant speed of water [m/s]
%    omega - angular velocity of propeller [rad/s]
%    thruster - struct with AT, BT, AQ and BQ (4-quadrant model parameters),
%           radius of propeller
%    rho - density of water [kg/m^3]
%
% Output:
%    Q - Generated torque of propeller [Nm]
%    T - Generated thrust of propeller [N]
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%


function [ Q,T ] = getPropellerTQ( Va,omega,thruster,rho )
beta = atan2(Va,0.7*omega*thruster.radius);
Ct = 0;
Cq = 0;
for i = 1:length(thruster.AT)
    Ct = Ct + thruster.AT(i)*cos(beta*(i-1))+thruster.BT(i)*sin(beta*(i-1));
    Cq = Cq + thruster.AQ(i)*cos(beta*(i-1))+thruster.BQ(i)*sin(beta*(i-1));
end
V07_2 = Va^2+(0.7*omega*thruster.radius)^2;
T = Ct*.5*rho*V07_2*pi*thruster.radius^2;
Q = Cq*.5*rho*V07_2*pi*2*thruster.radius^3;

end

