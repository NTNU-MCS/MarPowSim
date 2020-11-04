% omega_d = findDesiredPropellerSpeed(Td,thruster,rho)
%
% Finds desired propellerspeed from desired thrust.
% Assumes constant KT0 (sec 3.5. in Smogeli2006)
%
% Inputs:
%    Td  -  Desired thrust [N]
%    thruster - Struct of propeller properties
%    rho -  Density of fluid
%
% Output:
%    omega_d - angular velocity of propeller which makes the propeller
%    generate the desired thrust [rad/s]
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%

function [omega_d,P_d,Q_d] = findDesiredPropellerSpeed(Td,thruster,rho)


if (Td >= 0)
    omega_d = 2*pi*sqrt(Td/(rho*(2*thruster.radius)^4*thruster.KT0));
    P_d = thruster.KP0*Td^(1.5);
    Q_d = thruster.KQ0/thruster.KT0*2*thruster.radius*Td;
else
    omega_d = -2*pi*sqrt(-Td/(rho*(2*thruster.radius)^4*thruster.KT0r));
    P_d = thruster.KP0r*(-Td)^(1.5);
    Q_d = thruster.KQ0r/thruster.KT0r*2*thruster.radius*Td;
end