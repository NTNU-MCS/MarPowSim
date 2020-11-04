% busSymbolic()
%
% Generates m-files for bus voltage calculation.
% This is done by symbolic toolbox.
%
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%

syms vr vi er ei zr zi p q real
% vr real line-to-line bus voltage
% vi imaginary line-to-line bus voltage
% er real induced voltage
% ei imaginary induced voltage
% zr thevinin resistance
% zi thevinin reactance
% p active power
% q reactive power

% Line-to-neutral voltage
V = (vr + vi*1j)/sqrt(3);
E = er + ei*1j;
Z = zr + zi*1j;
S = p + q*1j;
I = (E-V)/Z;
VI = solve(0== imag(S*Z') - imag(3*(V*I')*Z'),vi);
eq = subs(0==real(S*Z') -real(3*(V*I')*Z'),vi,VI);
VR = solve(eq,vr);
matlabFunction(VR,'file','busVoltageVRnonZeroEr')
matlabFunction(VI,'file','busVoltageVInonZeroEr')


matlabFunction(simplify((VR(1)-VR(2))^2)>= 0,'file','busVoltageIsValid')

% When er = 0 the previous solution is invalid
E = ei*1j;
I = (E-V)/Z;
VR = simplify(solve(imag(S*Z') == imag(3*(V*I')*Z'),vr));
eq = subs(real(S*Z') == real(3*(V*I')*Z'),vr,VR);
VI = simplify(solve(eq,vi));
matlabFunction(VR,'file','busVoltageVRzeroEr')
matlabFunction(VI,'file','busVoltageVIzeroEr')

% Line-to-neutral voltage
V = (vr + vi*1j)/sqrt(3);
E = er + ei*1j;
Z = zr + zi*1j;
S = p + q*1j;
I = (E-V)/Z;
VR = simplify(solve(imag(S*Z') == imag(3*(V*I')*Z'),vr));
eq = subs(real(S*Z') == real(3*(V*I')*Z'),vr,VR);
VI = simplify(solve(eq,vi));
matlabFunction(VR,'file','busVoltageVRlowEr')
matlabFunction(VI,'file','busVoltageVIlowEr')