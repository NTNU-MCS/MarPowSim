% Class for testing TheveninEquivalent
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>


classdef theveninEquivalentTest < matlab.unittest.TestCase
    properties
        AbsTol = 1e-6;
        RelTol = 1e-6;
        e =[
            -0.377650112010776 - 0.932409321944982i
            -0.377650112010776 - 0.932409321944980i
            -0.377650112010776 - 0.932409321944980i
            -0.377650112010775 - 0.932409321944980i
            -0.377650112010733 - 0.932409321944873i
            -0.377650112010776 - 0.932409321944982i
            ];
        z = [  0.002500000000000 + 1.515150760975629i
            0.002500000000000 + 1.515150760975639i
            0.002500000000000 + 1.515150760975628i
            0.002500000000000 + 1.515150760975641i
            0.002500000000000 + 1.515150760975629i
            0.002500000000000 + 1.515150760975639i];
        Sb = [9100000
            9100000
            9100000
            9100000
            9100000
            9100000];
        ZT =  0.002500000000000 + 1.515150760975629i;
        ET =  -0.377650112010776 - 0.932409321944982i;
        SB_BUS =  54600000;
    end
    
    methods (Test)
        function testSixInput(testCase)
            on = [1,1,1,1,1,1];
            omega = zeros(6,1);
            [ et, zt, Sb_bus ] = TheveninEquivalent( testCase.e, testCase.z, testCase.Sb , on, omega );
            testCase.verifyEqual(et,testCase.ET,'RelTol',testCase.RelTol);
            testCase.verifyEqual(zt,testCase.ZT,'RelTol',testCase.RelTol);
            testCase.verifyEqual(Sb_bus,testCase.SB_BUS);
        end
        
        function testTwoPlusTwo(testCase)
            
            on = [1,1,1,0,0,0];
            omega = zeros(6,1);
            [ et1, zt1, Sb_bus1 ] = TheveninEquivalent( testCase.e, testCase.z, testCase.Sb , on,  omega);
            on = [0,0,0,1,1,1];
            [ et2, zt2, Sb_bus2 ] = TheveninEquivalent( testCase.e, testCase.z, testCase.Sb , on, omega );
            [ et, zt, Sb_bus ] = TheveninEquivalent( [et1 et2].', [zt1 zt2].', [Sb_bus1 Sb_bus2].' , [1 1], [1 1].' );
            
            testCase.verifyEqual(et,testCase.ET,'RelTol',testCase.RelTol);
            testCase.verifyEqual(zt,testCase.ZT,'RelTol',testCase.RelTol);
            testCase.verifyEqual(Sb_bus,testCase.SB_BUS);
        end
        
        function testSixInput2(testCase)
            e2 =[
                 -0.454578758176334 - 0.897416422408410i
 -0.454578758176334 - 0.897416422408410i
 -0.454578758176334 - 0.897416422408410i
 -0.454578758176335 - 0.897416422408411i
 -0.454578758176335 - 0.897416422408411i
 -0.454578758176334 - 0.897416422408410i
                ];
            z2 = [    0.002500000000000 + 1.512990518602113i
  0.002500000000000 + 1.512990518602113i
  0.002500000000000 + 1.512990518602113i
  0.002500000000000 + 1.512990518602113i
  0.002500000000000 + 1.512990518602113i
  0.002500000000000 + 1.512990518602113i];
            Sb2 = [9100000
                9100000
                9100000
                9100000
                9100000
                9100000];
            ZT2 =  0.002500000000000 + 1.512990518602113i;
            ET2 =  -0.454578758176334 - 0.897416422408410i;
            SB_BUS2 =  54600000;
            on = [1,1,1,1,1,1];
            omega = zeros(6,1);
            [ et, zt, Sb_bus ] = TheveninEquivalent( e2, z2, Sb2 , on, omega );
            testCase.verifyEqual(et,ET2,'RelTol',testCase.RelTol);
            testCase.verifyEqual(zt,ZT2,'RelTol',testCase.RelTol);
            testCase.verifyEqual(Sb_bus,SB_BUS2);
        end
        
    end
end
