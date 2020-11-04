%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [R,h,s,u,RF,RP,RT,uF,uP,uT,Cp,Cv,K] = GetThdynCombGasZach( P, T, F )
%The routine calculates the thermodynamic property of the combustion gas
%given pressure, temperature and the fuel air equivalent ratio.
% Input
%   P : Pressure (Pa)
%   T : Temperature (K)
%   F : Fuel-air equivalent ratio
% Output
%   R : Gas constant
%   h :	Specific enthalpy
%   s : Specific Entropy
%   u : Specific internal energy
%   RF : Partial derivative of R w.r.t. F
%   RP : Partial derivative of R w.r.t. P
%   RT : Partial derivative of R w.r.t. T
%   uF : Partial derivative of u w.r.t. F
%   uP : Partial derivative of U w.r.t. P
%   uT : Partial derivative of U w.r.t. T
%   CP : Specific heat at constant pressure
%   CV : Specific heat at constant volume
%   K  : Ratio of specific heats

FC0 = 1.02e-5;
PP = 1.01972e-5*P;
TT = 0.001*T;
V = (1-F)./(1+0.0698*F);

EP0 = 3.514956 - 0.005026*V;
EP1	= 0.131438 - 0.383504*V;
EP2	= 0.477182 + 0.185214*V;
EP3	= -0.287367 - 0.0694862*V;
EP4	= 0.0742561 + 0.0164041*V;
EP5	= -0.00916344 - 0.00204537*V;
EP6	= 0.000439896 + 0.00010161*V;
PA = EP0 + TT.*(EP1 + TT.*(EP2 + TT.*(EP3 + TT.*(EP4 + TT.*(EP5 + TT.*EP6)))));
PAT	= EP1 + TT.*(2*EP2 + TT.*(3.0*EP3 + TT.*(4.0*EP4 + ...
    TT.*(5.0*EP5 + TT.*6.0.*EP6))));
PAV	= -5.06e-3 + TT.*(-3.83504e-1 + TT.*(1.85214e-1 + TT.*(-6.94862e-2 + ...
    TT.*(1.64041e-2 + TT.*(-2.04537e-3 + TT.*1.01610e-4)))));
PB	= 2.972979 + 5.5314e-1*V + TT.*(5.83837e-2 - 4.85001e-1*V + ...
    TT.*(8.71349e-1 + 6.98183e-2*V + TT.*(-4.51556e-1 - 2.8772e-3*V + ...
    TT.*(1.09164e-1 - 6.93663e-4*V + TT.*(1.29754e-2 +9.20231e-5*V + ...
    TT.*(6.08234e-4 - 2.99679e-6*V))))));
FC1	= -8.05214e-1 + V.*(-4.00981e-1 + V.*(5.76989 + 6.0056e-2*V));
FC2	= 2.77105e-4 - 0.900711e-4*V;
FC3	= 6.42217e-4 - 0.983670e-4*V;
FC4	= 0.88680e-2 - 0.613100e-2*V;
FC5	= TT.*TT;
FC6 = exp(FC4./FC5)./TT;
FC7	= PP./TT;
FC8	= 1 + FC4./FC5;
ZZ	= FC7.*(FC3 - FC2.*FC6) + 1;
ZZV	= FC7.*(9.8367e-5 - FC6.*(9.00711e-5 - 6.131e-4*FC2./FC5));
ZZT	= FC7.*(FC2.*FC6.*(FC8 + 1) - FC3)./TT;
ZZP	= (FC3 - FC2.*FC6)./TT;
HH	= PA-ZZT.*TT;
HHP	= -ZZT.*TT./PP;
HHT	= -FC7./TT.*(FC3 - 4*FC2.*FC6.*(FC8.*FC8+FC4./FC5)) + PAT;
HHV	= FC7.*(-9.8367e-5 - 2*FC6.*(FC8.*(-9.00711e-5 - 6.131e-3*FC2./FC5) - ...
    6.131e-3*FC2./FC5))+PAV;
SS	= (3.5566 - 6.59e-2*V).*log(T) + PB - FC1 - log(PP);
SS1	= SS - FC7.*FC2.*FC6.*FC8;
FC11 = 8314.7*(F/3 + 4.773384)./(9.344254*F + 138.2266);
FC21 = -1.471965*(F/3 + 4.773384)./(9.344254*F + 138.2266);
FC31 = -1.0698./((1 + 0.0698*F).*(1 + 698*F));
R = FC11.*ZZ;
h = FC11.*T.*HH;
s = FC11.*SS1;
u = h - R.*T;
RF = FC11.*(ZZV.*FC31-ZZ.*FC21);
RP = FC11.*ZZP.*FC0;
RT = FC11.*ZZT*1.0e-3;
uF = FC11.*T.*((HHV-ZZV).*FC31-(HH-ZZ).*FC21);
uP = FC11.*T.*(HHP-ZZP).*FC0;
uT = FC11.*(HH - ZZ + TT.*(HHT - ZZT));
Cp = FC11.*(HH+TT.*HHT);
Cv = uT + uP.*(R./T + RT)./(R./P - RP);
K = Cp./Cv;

end

