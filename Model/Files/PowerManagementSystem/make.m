% make()
%
% Compiles power management sfunction. Does only compile changed source 
% files and link if any source file have changed
%
% Inputs:
%    'all' - Compiles all files
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%


function make(outDir,varargin)
objextension = 'obj';
all = false;
for i = 1:length(varargin)
    if (strcmp(varargin{i},'all'))
        all = 1;
    else
        error('Parameter number %d is unknown',i);
    end
end

if isunix() || ismac()
    objextension = 'o';
end

if ~exist('binMatlab','dir')
    mkdir('binMatlab')
end

tmp = dir('src/*.cpp');
nameO = '';
disp('Compiling')
mex -g -c -outdir binMatlab sfun_PMS.cpp
for i=1:length(tmp)
    t = tmp(i);
    nameO = sprintf('%s binMatlab/%s.%s',nameO,t.name(1:(end-4)),objextension);
    target = sprintf(t.name(1:(end-4)));
    if (all || shouldBeCompiled(target,objextension))
        eval(sprintf('mex -g -c -outdir binMatlab src/%s.cpp',t.name(1:(end-4))));
    end
    fprintf('.');
end
fprintf('\nLinking\n');
eval(sprintf('mex -g -outdir %s -IbinMatlab sfun_PMS.cpp %s',outDir,nameO));
end

function flag = shouldBeCompiled(target,objextention)
flag = true;
if (~exist(sprintf('binMatlab/%s.%s',target,objextention),'file'))
    return;
end
targetO = dir(sprintf('binMatlab/%s.%s',target,objextention));
targetOtime = targetO.datenum;
targetCpp = dir(sprintf('src/%s.cpp',target));
if (targetCpp.datenum > targetOtime)
    return;
end
flag = false;
return;
end