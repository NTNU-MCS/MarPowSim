d = dir;
isub = [d(:).isdir]; %# returns logical vector
nameFolds = {d(isub).name}';
nameFolds(ismember(nameFolds,{'.','..','Old'})) = [];
for i = 1:length(nameFolds);
    folder = nameFolds{i};
    cd(folder)
    exampleFolder = pwd();
    initFileName = sprintf('init%s',folder);
    outDir = sprintf('%s/Files',exampleFolder);
    cd ../../Model/Files/ThrusterAllocation/src/
    make(exampleFolder,initFileName,sprintf('%s/Files',exampleFolder))
    cd ../../PowerManagementSystem/
    make(outDir)
    cd(exampleFolder)
    cd ..
end