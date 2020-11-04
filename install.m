%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
folder = {'Model','Model/Files','Model/Files/EngineData','Model/Files/EngineSfuncMex','Model/Files/SFuncMex','Model/Submodels','Model/Submodels/figs'};
for i = 1:length(folder)
    addpath(sprintf('%s/%s',pwd,folder{i}));
end


str = input('The path needs to be saved, else this script must be runned each time matlab is started. Do you want to save the new path? [Y/n]: ','s');
if (length(str) == 0 || (length(str) == 1 && (str == 'Y' || str == 'y')))
    savepath;
    disp('Saved path')
end
