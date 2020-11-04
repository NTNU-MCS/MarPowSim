%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014-2017	Torstein Aarseth Bø <torstein.bo@sintef.no>

function VI = busVoltageVInonZeroEr(ei,er,p,q,vr,zi,zr)
%BUSVOLTAGEVINONZEROER
%    VI = BUSVOLTAGEVINONZEROER(EI,ER,P,Q,VR,ZI,ZR)

%    This function was generated by the Symbolic Math Toolbox version 5.10.
%    11-Jun-2014 16:56:09

t2 = sqrt(3.0);
VI = (ei.*vr-p.*t2.*zi.*(1.0./3.0)+q.*t2.*zr.*(1.0./3.0))./er;