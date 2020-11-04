% Test cases for test of busVoltage
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.08.05	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%               <date>	<author> <description>
%

classdef testBusVoltage < matlab.unittest.TestCase
    properties
        AbsTol = 1e-6;
        RelTol = 1e-6;
    end
    
    methods (Test)
        function testVoltage(testCase)
            rng(683);
            for i = 1:100
                ET = (1+rand*.01)*exp(1j*2*pi*rand);
                ZT =  0.002500000000000 + 1.512990436525471i;
                Sb_bus = 54600000;
                Pbus =  Sb_bus*rand;
                Qbus =  Sb_bus*(.5-rand);
                [V, ~, isInputValid] = busVoltage( ET, ZT, Sb_bus, Pbus, Qbus);
                if isInputValid
                    V = V/sqrt(3);
                    I = (ET-V)/ZT;
                    S = (3*V*conj(I))*Sb_bus;
                    % Check if the solution is valid
                    testCase.verifyEqual(S,Pbus+1j*Qbus,'RelTol',testCase.RelTol);
                end
            end
        end
    end
    
    methods (Static)
        function runTests()
            o =  testBusVoltage;
            o.run();
        end
    end
end
