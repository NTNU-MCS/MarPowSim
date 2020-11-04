%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [ rho_a ] = GetAirDensity(p,T)
%The function calculates the air density based on equation of state
%proposed by Kadoya. 
% Input
%   p : pressure in Pa
%   T : Temperature in K
% Output
%   rho_a : density of the air
% Validity : 300~2000K and upto 100MPa. 
% Ref
%   Kadoya, K., N. Matsunaga, et al. (1985). "Viscosity and Thermal 
%   Conductivity of Dry Air in the Gaseous Phase." J. Phys. Chem. Ref. Data 14(4).
% Created by

R_u = 8.314; %Universal Gas constant J/(molK)

rho_ideal = p/T/R_u;

T_B = 340; % (K)
B1 = 20.5e-6; % (10e-6m3/mol)
b = [-0.00168785;-0.0223299;-0.170400;-1.94783;2.16059;-0.0222430];
c = [2.50287;-5.52109;4.29631;1.19665];

TT = zeros(6,1);

rho_error = 1;
rho = rho_ideal;
tau = T/T_B;
TT(1) = 1/(tau)^4;
for j=2:6
    TT(j) = TT(j-1)*tau;
end;
while rho_error > 1e-3
    Z = rho_ideal/rho;
    B = B1*b'*TT;
    C = B1^2*c'*TT(2:5);
    f = Z - 1 - B*rho - C*rho^2;
    dfdrho = -rho_ideal/rho^2 - B - 2*C*rho;
    drho = -f/dfdrho;
    rho_new = max(0.1*rho,rho + drho);
    rho_error = abs(rho_new-rho)/rho;
    rho = rho_new;
end;

rho_a = rho*28.97/1000;

%end

