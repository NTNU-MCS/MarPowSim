d = dir;
isub = [d(:).isdir]; %# returns logical vector
nameFolds = {d(isub).name}';
nameFolds(ismember(nameFolds,{'.','..','Old'})) = [];
for i = 1:length(nameFolds);
    folder = nameFolds{i};
    delete(sprintf('%s/*.mex*',folder),sprintf('%s/Files/*.mex*',folder));
end