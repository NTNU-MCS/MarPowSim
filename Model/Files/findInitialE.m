% E = findInitialE(generators,connectedGensets,VT0,p0,q0)
%
% Find the initial induced voltage of generator sets which gives desired
% loadsharing and bus voltage
%
% Inputs:
%    generators - Struct of generator parameters
%    connectedGensets - Vector of bools, true if generator is connected
%    VT0 - Intial bus voltage
%    p0 - intial per unit active power of bus
%    q0 - intial per unit reactive power of bus
%
% Outputs:
%    E - Initial induced voltage
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%

function E = findInitialE(generators,connectedGensets,VT0,p0,q0)
nThrusters = length(generators);

VT0 = mean(VT0);
nr = zeros(sum(connectedGensets),1);
Sb = zeros(nThrusters);
z = Sb;
n = 1;
for i = 1:length(connectedGensets)
    if (connectedGensets(i))
        nr(n) = i;
        n = n +1;
    end
    g = generators(i);
    Sb(i) = g.Sb;
    z(i) = g.r+g.xs*1j;
end

% Find base power
Sb_bus = sum(Sb(nr));
sb = Sb./Sb_bus;

%Calculate the Thevenin imepdance
ZT = 1/sum(sb(nr)./z(nr));
VT0 = VT0/sqrt(3);
phi = -acos(p0/sqrt(p0^2+q0^2));
I = (abs(p0+1j*q0)/abs(3*VT0))*exp(1j*phi);
E = ones(nThrusters,1);
E(nr) = VT0+ZT*I;
assert(abs(p0+q0*1j - 3*VT0*I')<1e-14);
end
