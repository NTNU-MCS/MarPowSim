%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [ l ] = makePulseTrain( lowPulse, highPulse,lengthLow,lengthHigh,offset,t)
%MAKEPULSETRAIN Summary of this function goes here
%   Detailed explanation goes here
lengthTot = lengthLow+lengthHigh;
l = lowPulse*t;
dt = t(2)-t(1);
pulse = highPulse*[ones(lengthHigh/dt,1);zeros(lengthLow/dt,1)].'+lowPulse*[zeros(lengthHigh/dt,1);ones(lengthLow/dt,1)].';
i0 = floor(mod((offset-t(1))/lengthTot,1)*lengthTot/dt)+1;
for i=1:(lengthTot/dt):(length(t)-(lengthTot/dt))
    l(:,i0+(i-1)+(1:lengthTot/dt)) = pulse;
end
i = i0+i+(lengthTot/dt);
n = length(l(1,i:end));
l(:,i:end) = pulse(:,1:n);
end

