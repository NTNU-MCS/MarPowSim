% omega_d = findDesiredPropellerSpeed(Td,thruster,rho)
%
% Finds estimate of thruster force from propeller speed.
% Assumes constant KT0 (sec 3.5. in Smogeli2006)
%
% Inputs:
%    omega - shaft speed of propeller
%    thruster - Struct of propeller properties
%    rho -  Density of fluid
%
% Output:
%    T_est - estimated thrust
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Created:  	2014.12.15	Torstein Ingebrigtsen Bø
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%

function T_est = estimatedThrusterForce(omega,thruster,rho)
    T_est = sign(omega)*(omega/(2*pi))^2*(rho*(2*thruster.radius)^4*thruster.KT0);