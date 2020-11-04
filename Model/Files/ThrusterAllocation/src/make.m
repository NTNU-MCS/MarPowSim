% make()
%
% Compiles thruster allocation sfunction. Does only compile changed source
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
function make(initFileDir,initFileName,outdir,varargin)
all = 0;
acadoFolder = '';
objExtension = '';
CC = []; % Compiler
%SingularityAvoidance.make(initFileDir,initFileName)

for i = 1:length(varargin)
    if (strcmp(varargin{i},'all'))
        all = 1;
    else
        error('Parameter number %d is unknown',i);
    end
end

os = 'u'; % u = unknown, w = windows, m = mac, l = linux;
if isunix() && ~ismac()
    acadoFolder = '/home/torstein/programmer/acado';
    objExtension = 'o';
    CC = 'gcc-4.4';
    os = 'l';
elseif ispc();
    os = 'w';
    candAcadoFolder = {'C:/Users/Michelicious/Documents/acado',...
        'C:/Users/torstebo/Documents/doktor/acado',...
        'C:\Users\torst_000\Documents\acado',...
        'D:/acado'};
    for i = 1:length(candAcadoFolder)
        if exist(candAcadoFolder{i},'dir')
            acadoFolder = candAcadoFolder{i};
            break
        end
    end
    objExtension = 'obj';
elseif ismac()
    os = 'm';
    acadoFolder = '/Users/torsteinibo/Documents/programs/acado';
    objExtension = 'o';
end


targets = {'sfun_thrust_allocation','ThrustAllocation','simulinkGetters','singularity_cost','singularity_cost_gradient','singularity_cost_wrapper'};

if ~exist('../Matlab','dir')
    mkdir('../Matlab')
end

flag = false;
for i = 1:length(targets)
    if(compile(targets{i},all,acadoFolder,objExtension,CC))
        flag = true;
    end
end
obj = '';
for i = 1:length(targets);
    obj = sprintf('%s ../Matlab/%s.o',obj,targets{i});
end
if (~flag && ~all && 0)
    disp('Nothing to do')
else
    fprintf('Linking\n')
    if strcmp(os,'w')
        
        % BUG should link to library but I cannot manage to do it!
        acadoBinFolder = sprintf('%s\\interfaces\\matlab\\bin\\acado',acadoFolder);
        acadoObj = {'*','\casadi\*','\qpOASES\*'};
        %acadoObj = findFilesInFolder(acadoBinFolder,objExtension);
        obj = '';
        for i = 1:length(targets);
            obj = sprintf('%s ..\\Matlab\\%s.obj',obj,targets{i});
        end
        for i = 1:length(acadoObj);
            obj = sprintf('%s %s\\%s.obj',obj,acadoBinFolder,acadoObj{i});
        end
        
        
        eval(sprintf('mex %s -g -outdir ''%s''',obj,outdir));
    else
        flags = 'CFLAGS="$CFLAGS -Wall -std=c++0x" -cxx -lacado_toolkit_s -lpthread';
        if ~isempty(CC)
            flags = sprintf('%s CC="%s"',flags,CC);
        end
        
        eval(sprintf('mex -lacado_toolkit_s %s %s -cxx  -lpthread -outdir ''%s''',flags,obj,outdir));
    end
    
end
end

%% Check if a file have changed since it was compiled
%% target - filename of source code without extension
%% objextention - filename extention of objectfile
function flag = shouldBeCompiled(target,objextention)
flag = true;
if (~exist(sprintf('../Matlab/%s.%s',target,objextention),'file'))
    return;
end
targetO = dir(sprintf('../Matlab/%s.%s',target,objextention));
targetOtime = targetO.datenum;
targetCpp = dir(sprintf('%s.cpp',target));
if (targetCpp.datenum > targetOtime)
    return;
end
flag = false;
return;
end

%% Compiles file
%% target - filename without extension
%% all - true if file should be compiled even if it is not changed
%% acadofolder - path to root path of acado
%% objExtension - file extension of object file
%% CC - compiler or empty if default compiler
function flag = compile(target,all,acadoFolder,objExtension,CC)
flag = false;
if (all || shouldBeCompiled(target,objExtension))
    fprintf('Compiling %s\n',target);
    flags = 'CFLAGS="$CFLAGS -Wall -std=c++11 -stdlib=libstdc++"';
    if ~isempty(CC)
        flags = sprintf('%s CC="%s"',flags,CC);
    end
    eval(sprintf('mex  %s -g -I"%s" -I"%s/acado" -I"%s/external_packages" %s.cpp -g -c -outdir ''../Matlab/''',flags,acadoFolder,acadoFolder,acadoFolder,target))
    flag = true;
end
end

function obj = findFilesInFolder(folder,objExtension)
        files = dir(folder);
        obj = '';
        for i = 1:length(dir)
            f = files(i);
            if ~f.isdir
                nameExtension = strsplit(f.name,'.');
                if strcmp(nameExtension{end},objExtension)
                    obj = sprintf('%s %s/%s',obj,folder,f.name);
                end
            elseif ~strcmp(f.name,'.') && ~strcmp(f.name,'..')
                subFolder = sprintf('%s/%s',folder,f.name);
                sprintf('%s %s',obj,findFilesInFolder(obj,subFolder,objExtension))
            end
        end
end

