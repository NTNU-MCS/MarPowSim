% runalltests()
%
% Run all unit tests in matlab
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Licensed under GPL-3.0
%    Revised:	<date>	<author> <description>
%               <date>	<author> <description>
%

run(theveninEquivalentTest);
run(testBusVoltage);
addpath('thruster');
run(thrusterTestCase)