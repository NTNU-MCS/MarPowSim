% [ et, zt, Sb_bus, onOut, omegaAvg ] = TheveninEquivalent( e, z, Sb, on, omega )
%
% Finds the Thevenin equivalent circuit of voltage
% sources + impedance in parallel
%
% Input:
%   e - induced voltage as phasors in per unit
%   z - impedance of voltage source in per unit
%   Sb - base power of generator in W
%   on - vector of same length as e and z with 1 if source is connected en
% zero else
%   omega - frequency of gensets in per unit.
% 
% Output:
%   et - thevenin equivalent voltage source in per unit
%   zt - thevenin equivalent impedance in per unit
%   Sb_bus - base power of thevenin equivalent circuit
%   onOut - 1 if one or more gensets are connected, 0 if not.
%   omegaAvg - average frequency of connected gensets
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>


function [ et, zt, Sb_bus, onOut, omegaAvg ] = TheveninEquivalent( e, z, Sb, on, omega ) %#codegen 
 %#codegen 



m = sum(on);
if (m == 0)
    onOut = false;
    et = 0;
    zt = 0;
    Sb_bus = 0;
    omegaAvg = 0;
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

%Calculate the Thevenin imepdance
zt = 1/sum(sb(nr)./z(nr));

% Calculate the Thevenin voltage
% http://www.diva-portal.org/smash/get/diva2:648679/FULLTEXT01.pdf
% Page 21.
et = sum(sb(nr).*e(nr)./z(nr))*zt;

% Calculates avarage frequency
omegaAvg = sum(sb(nr).*omega(nr));
end

