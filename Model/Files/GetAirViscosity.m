%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [ mu_a ] = GetAirViscosity(rho,T)
%The function calculates the air viscosity based on correlation
%proposed by Kadoya. 
% Theory
%   \mu\left(p,T\right) = H \left(\mu_0(T_r) + \Delta\mu(\rho_r)\right)
%   \mu_0(T) = A_1 T + A_{0.5} T^{0.5} + \sum_{i=0}^{-4} A_i T_r^i
%   \mu(\rho_r) = \sum_{i=1}^{4} B_i rho_r^i
%   T_r = T/T_{ref}     \rho_r = rho/rho_{ref}
%   
% Input
%   rho : pressure in kg/m3
%   T : Temperature in K

% Output
%   mu_a : viscosity of the air in Pa-s
% Validity : 300~2000K and upto 100MPa. 
% Ref
%   Kadoya, K., N. Matsunaga, et al. (1985). "Viscosity and Thermal 
%   Conductivity of Dry Air in the Gaseous Phase." J. Phys. Chem. Ref. Data 14(4).
% Created by Kevin Koosup Yum (NTNU) on 23 May, 2013

mm = size (rho);
nn = size (T);

if nn(1) >= nn(2)
    n = nn(1);
else
    n = nn(2);
    T = T';
end;
if mm(1) <= mm(2)
    rho = rho';
end;

rho_ref =   314.3;
T_ref = 132.5;
H = 6.1609;
A = [0.128517;-1;-0.709661;0.662534;-0.197846;0.00770147];
A_05 = 2.60661;
B = [0.465601;1.26469;-0.511425;0.27460];
rho_r = rho/rho_ref;
T_r = T/T_ref;
R_u = 8.314; %Universal Gas constant J/(molK)

TT = zeros(n,6);
rhor = zeros(n,4);
TT(:,1)  = T_r;
rhor(:,1) = rho_r;
for i = 2:4
    TT(:,i) = TT(:,i-1)./T_r;
    rhor(:,i) = rhor(:,i-1).*rho_r;
end
TT(:,5) = TT(:,4)./T_r;

mu_a = H*(TT*A + A_05*T_r.^0.5 + rhor*B)/1e6; 

end

