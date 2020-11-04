% runsimulation()
%
% Run simulation
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.08.05	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	2020.11.03  Torstein Aarseth Bø <torstein.bo@sintef.no>
%
initDrillingRigMSStoolbox;

% Load the model
mdl = 'drillingRigMss';
load_system(mdl);


% Run the simulation
tic;
simOut = sim(mdl,'SaveOutput','on',...
        'SaveFormat','StructureWithTime',...
        'ReturnWorkspaceOutputs','on',...
        'StartTime','-500','StopTime', '-400');
toc;
