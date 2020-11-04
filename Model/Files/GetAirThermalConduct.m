%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [ lambda_a ] = GetAirThermalConduct(rho,T)
%The function calculates the air thermal conductivity based on correlation
%proposed by Kadoya. 
% Theory
%   \lambda\left(T,\rho\right) = \Lambda \left(\lambda_0(T_r) + \Delta\lambda(\rho_r)\right)
%   \lambda_0(T_r) = C_1 T + C_{0.5} T^{0.5} + \sum_{i=0}^{-4} C_i T_r^i
%   \Delta\lambda(\rho_r) = \sum_{i=1}^{5} B_i rho_r^i
%   T_r = T/T_{ref}     \rho_r = rho/rho_{ref}
%   
% Input
%   rho : pressure in kg/m3
%   T : Temperature in K

% Output
%   lambda_a : thermal conductivity of the air in W/(mK)
% Validity : +-1% in high temperature (>370K)
% Ref
%   Kadoya, K., N. Matsunaga, et al. (1985). "Viscosity and Thermal 
%   Conductivity of Dry Air in the Gaseous Phase." J. Phys. Chem. Ref. Data 14(4).
% Created by Kevin Koosup Yum (NTNU) on 29 August, 2013



mm = size (rho);
nn = size (T);

if nn(1) >= nn(2)
    n = nn(1);
else
    n = nn(2);
    T = T';
end;
if mm(2) > mm(1)
    rho = rho';
end;


rho_ref =   314.3;
T_ref = 132.5;
Lambda = 25.9778e-3;
C = [0.239503;1;-1.92615;2.00383;-1.07553;0.229414];
C_05 = 0.00649768;
D = [0.402287;0.356603;-0.163159;0.138059;-0.020172];
rho_r = rho/rho_ref;
T_r = T/T_ref;

TT = zeros(n,6);
rhor = zeros(n,5);
TT(:,1)  = T_r;
rhor(:,1) = rho_r;
for i = 2:5
    TT(:,i) = TT(:,i-1)./T_r;
    rhor(:,i) = rhor(:,i-1).*rho_r;
end
TT(:,6) = TT(:,5)./T_r;
lambda_a = Lambda*(TT*C + C_05*T_r.^0.5 + rhor*D);

end

