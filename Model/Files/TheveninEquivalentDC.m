%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2013-2017 Torstein Aarseth Bø

 %#codegen
function [ et, zt, Sb_bus, onOut] = TheveninEquivalentDC( e, z, Sb, on) %#codegen 
 %#codegen 
%THEVENINEQUIVALENT Finds the Thevenin equivalent circuit of voltage
%sources + impedance in parallel
% e is induced voltage as phasors in per unit
% z is impedance of voltage source in per unit
% Sb is base power of generator in W
% on is vector of same length as e and z with 1 if source is connected en
% zero else

m = sum(on);
if (m == 0)
    onOut = false;
    et = 0;
    zt = 0;
    Sb_bus = 0;
    return;
else
    onOut = true;
end
maxM = 100;
assert(m<maxM);
nr = zeros(1,m);
n = 1;
for i = 1:length(on)
    if (on(i))
        nr(n) = i;
        n = n +1;
    end
end

% Find base power
Sb_bus = sum(Sb(nr));
sb = Sb./Sb_bus;

% %Calculate the Thevenin impedance
% zt = 1/sum(ones(nr)./z(nr));
% 
% % Calculate the Thevenin voltage
% % http://www.diva-portal.org/smash/get/diva2:648679/FULLTEXT01.pdf
% % Page 21.
% et = sum(sb(nr).*e(nr)./z(nr))/sum(sb(nr)./z(nr));

%Calculate the Thevenin imepdance
zt = 1/sum(sb(nr)./z(nr));

% Calculate the Thevenin voltage
% http://www.diva-portal.org/smash/get/diva2:648679/FULLTEXT01.pdf
% Page 21.
et = sum(sb(nr).*e(nr)./z(nr))*zt;

end

