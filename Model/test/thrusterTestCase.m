% Class for testing thruster
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>

classdef thrusterTestCase < matlab.unittest.TestCase
    properties
        thruster;
        rho;
    end
    
    methods(TestMethodSetup)
        function init(testCase)
            testCase.rho = 1025;
        end
        
        function createThruster(testCase)
            init;
            testCase.thruster = thrusters(1);
        end
    end
    methods (Test)
        function testIfRunGetPropellerTQ(testCase)
            Va = 1;
            omega = 2.05*2*pi;
            [Q,T] = getPropellerTQ(Va,omega,testCase.thruster,testCase.rho);
        end
        function testGetPropellerTQ(testCase)
            Va = 0;
            T_d = 4e4;
            omega_d = findDesiredPropellerSpeed(T_d,testCase.thruster,testCase.rho);
            [Q,T] =getPropellerTQ(Va,omega_d,testCase.thruster,testCase.rho);

            testCase.assertEqual(T,T_d,'RelTol',.1);
        end
        function testGetPropellerTEstimate(testCase)
            T_d = 4e4;
            omega_d = findDesiredPropellerSpeed(T_d,testCase.thruster,testCase.rho);
            T =estimatedThrusterForce(omega_d,testCase.thruster,testCase.rho);

            testCase.assertEqual(T,T_d,'RelTol',1e-6);
        end
    end
    
end

