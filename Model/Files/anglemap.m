% [x] = anglemap(y)
%
% Map angles between -pi and pi;
%
% Inputs:
%    y - angle
%
% Output:
%    x - angle mapped to -pi to pi
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%

function [ x ] = anglemap( y )
% Map all angles between -pi and pi
x = mod(y,2*pi);
x = x - (x>pi)*2*pi;
end

