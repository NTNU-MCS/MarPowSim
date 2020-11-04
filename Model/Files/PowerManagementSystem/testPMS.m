%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2013-2017 Torstein Aarseth Bø



init
sim PMStest


sum(powerAvailableHP.Data(1,:) ~= HighPriorityDesiredPower)
sum(powerAvailableDP.Data(1,:) ~= DPDesiredPower)
sum(powerAvailableLP.Data(1,:) ~= LowPriorityDesiredPower)
