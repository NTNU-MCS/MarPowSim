%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014-2017	Torstein Aarseth Bø <torstein.bo@sintef.no>

function [highPriorityLoad,lowPriorityLoad ] = addLoad( load ,priority,highPriorityLoad,lowPriorityLoad )
%ADDLOAD Summary of this function goes here
%   Detailed explanation goes here
if (priority)
    highPriorityLoad = highPriorityLoad + load;
else
    lowPriorityLoad = lowPriorityLoad + load;
end

end

