% setColorScheme(mdl)
%
% Sets the background color of simulink blocks to follow Roger Skjetne
% color scheme.
%
% Inputs:
%    mdl - filename of simulink file
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	<date>	<author> <description>
%    		<date>	<author> <description>
%


function setColorScheme(mdl)
%set_param(mdl,'Lock','off')
blocks = find_system(mdl,'LookUnderMasks','all');
for i = 2:length(blocks)
    blc = blocks{i};
    type = get_param(blc,'BlockType');
    
    switch type
        case {'Inport','Constant'}
            color = 'green';
        case {'Outport','Terminator'}
            color = 'red';
        case 'Goto'
            color = 'ORANGE';
        case {'Integrator'}
            color = 'cyan';
        case 'From'
            color = 'MAGENTA';
        otherwise
            
            switch get_param(blc,'Tag')
                case 'GotoMod'
                    color = 'cyan';
                otherwise
                    refBlock = get_param(blc,'ReferenceBlock');
                    if regexp(refBlock,'Logic and Bit\nOperations','once')
                        color = 'gray';
                    else
                        color = 'yellow';
                    end
            end
    end
    set_param(blocks{i},'BackgroundColor',color)
    set_param(blocks{i},'ForegroundColor','black')
end
end