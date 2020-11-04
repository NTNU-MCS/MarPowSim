%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2013-2017 Torstein Aarseth Bø

%#codegen
function [out,isInputValid]= switchboardCentral(powerLoads,switchboard2bus,nbus,theveninEquivalent)
%#codegen 
nSwitchboards = length(switchboard2bus);
assert(nbus<=nSwitchboards)
isInputValid = true;
omegaAvg = ones(nbus,1);
thetaElectric = zeros(nbus,1);
v = complex(ones(nSwitchboards,1));


for bus=1:nbus    
    % Sum loads
    Pbus = powerLoads(bus).p;
    Qbus = powerLoads(bus).q;
    t=theveninEquivalent(bus); 
    if (t.Connected)
                                       
        [Vbus, ~, isInputValidBus] = busVoltage(t.E, t.Z, t.Sb, Pbus, Qbus );
        
        isInputValid = isInputValid && isInputValidBus;

        % Calculate the electrical angle
        thetaElectric(bus) = angle(Vbus);
        omegaAvg(bus) = t.omega;
        if (abs(Vbus) <=0.0001)
           Vbus = Vbus*0.0001/abs(Vbus);
        end
         v(bus) = Vbus;
    else
        % Dead bus. 
        % Set values to rated, such that engines can be connected
        v(bus) = 1;

        % Rearange output
        omegaAvg(bus) = 1;
        thetaElectric(bus) = 0;
    end
end
out = repmat( struct( 'voltage',1j,'omega',1,'thetaElectric',1 ), nSwitchboards, 1 );
for s = 1:nSwitchboards
    i = switchboard2bus(s);
    out(s) = struct('voltage',v(i),'omega',omegaAvg(i),'thetaElectric',thetaElectric(i));
end