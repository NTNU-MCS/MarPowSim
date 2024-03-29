%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014-2017	Torstein Aarseth Bø <torstein.bo@sintef.no>

function VI = busVoltageVIlowEr(ei,er,p,q,zi,zr)
%BUSVOLTAGEVILOWER
%    VI = BUSVOLTAGEVILOWER(EI,ER,P,Q,ZI,ZR)

%    This function was generated by the Symbolic Math Toolbox version 5.10.
%    11-Jun-2014 16:55:15

t2 = ei.^2;
t3 = sqrt(3.0);
t4 = er.^2;
t5 = t2+t4;
t6 = 1.0./t5;
t7 = ei.*t2.*t3.*3.0;
t8 = p.^2;
t9 = zi.^2;
t10 = q.^2;
t11 = zr.^2;
t12 = t2.^2;
t13 = t12.*9.0;
t14 = t4.^2;
t15 = t14.*9.0;
t16 = t2.*t4.*1.8e1;
t17 = p.*q.*zi.*zr.*8.0;
t18 = t13+t15+t16+t17-t8.*t9.*4.0-t10.*t11.*4.0-p.*t2.*zr.*1.2e1-p.*t4.*zr.*1.2e1-q.*t2.*zi.*1.2e1-q.*t4.*zi.*1.2e1;
t19 = sqrt(t18);
t20 = ei.*t3.*t4.*3.0;
t21 = er.*q.*t3.*zr.*2.0;
VI = [t6.*(t7+t20+t21-ei.*t3.*t19-er.*p.*t3.*zi.*2.0).*(1.0./6.0);t6.*(t7+t20+t21+ei.*t3.*t19-er.*p.*t3.*zi.*2.0).*(1.0./6.0)];
