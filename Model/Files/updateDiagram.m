% updateDiagram()
%
% This script should be runned after a change of block diagram (either
% change of number of components or how the components are connected to
% switchboards). This block makes goto and from blocks
%
% Inputs:
%     mdl - name of model, default is 'motion'
%     thrusters - struct with thrusters parameter
%     generators - struct with generators parameter
% Outputs:
%     nSwitchboard - number of switchboards detected in the simulink
%                       diagram
%     nGeneratorSet - number of generator sets detected in the simulink
%                       diagram
%     nThruster - number of thrusters detected in the simulink diagram
%
%
%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	2014.07.02	Torstein Aarseth Bø <torstein.bo@sintef.no>
%    Revised:	2014.08.11	Michel Rejani Miyazaki
%                   Modifying the file to update both DC and AC gensets
%    		<date>	<author> <description>
%

function [nSwitchboard, nGeneratorSet, nThruster] =  updateDiagram(varargin)
%init;

if length(varargin) == 3
   mdl = varargin{1};
   thrusters = varargin{2};
   generators = varargin{3};
elseif isempty(varargin) == 1
   mdl = 'motion';
   init
else
   error('Number of inputs must be either 0 or 3')
end
load_system(mdl);
PMS = find_system(mdl,'LookUnderMasks','all','Tag','PowerManagementSystem'); PMS = PMS{1};


%% Update Switchboards
Switchboard = find_system(mdl,'LookUnderMasks','all','Tag','Switchboard');
nSwitchboard = length(Switchboard);
Switchboard2Bus = find_system(mdl,'LookUnderMasks','all','Tag','Switchboard2Bus');
set_param(Switchboard2Bus{1},'nSwitchboards',int2str(nSwitchboard));


uniqueIdTest = ones(1,nSwitchboard);
for i = 1:nSwitchboard
    swbId = str2double(get_param(Switchboard{i},'swbId'));
    if (swbId < 1 || swbId>nSwitchboard)
       error('Switchboard must have an unique ID between 1 and number of Switchboards. Error with switchboard: "%s" Switchboard id:%d',Switchboard{i},swbId);
    end
    
    if uniqueIdTest(swbId) == 1
        uniqueIdTest(swbId) = 0;
    else
        error('There exists multiple swithcboard with the same ID, it must be unique. Switchboard: "%s" has not a unique id',Switchboard{i});
    end
end

MergerLoadsCentral = find_system(mdl,'LookUnderMasks','all','Tag','FromLoads/MergerLoads'); MergerLoadsCentral = MergerLoadsCentral{1};
MergerLoadsCentralName = get_param(MergerLoadsCentral,'Name');

%% Update Load Others
LoadOther = find_system(mdl,'LookUnderMasks','all','Tag','Load Others');
LoadOtherFrom = find_system(PMS,'LookUnderMasks','all','Tag','PowerManagementSystem/LoadOthersFrom'); LoadOtherFrom = LoadOtherFrom{1};
LoadOtherFromCentral = find_system(mdl,'LookUnderMasks','all','Tag','FromLoads'); LoadOtherFromCentral = LoadOtherFromCentral{1};
nLoadOthers = length(LoadOther);

if (nLoadOthers == 0)
    error('There must at least be one LoadOthers block in the diagram');
end

PowerDesiredLowPriorityFrom = find_system(LoadOtherFrom,'Tag','PowerManagementSystem/LoadOthersFrom/PowerDesiredLowPriority');
PowerDesiredLowPriorityFrom = PowerDesiredLowPriorityFrom{1};
PowerDesiredLowPriorityFromName = get_param(PowerDesiredLowPriorityFrom,'Name');
set_param(PowerDesiredLowPriorityFrom,'NumInputs',int2str(nLoadOthers));

PowerLowPriorityFrom = find_system(LoadOtherFrom,'Tag','PowerManagementSystem/LoadOthersFrom/PowerLowPriority');
PowerLowPriorityFrom = PowerLowPriorityFrom{1};
PowerLowPriorityFromName = get_param(PowerLowPriorityFrom,'Name');
set_param(PowerLowPriorityFrom,'NumInputs',int2str(nLoadOthers));

PowerHighPriorityFrom = find_system(LoadOtherFrom,'Tag','PowerManagementSystem/LoadOthersFrom/PowerHighPriority');
PowerHighPriorityFrom = PowerHighPriorityFrom{1};
PowerHighPriorityFromName = get_param(PowerHighPriorityFrom,'Name');
set_param(PowerHighPriorityFrom,'NumInputs',int2str(nLoadOthers));

PowerDesiredHighPriorityFrom = find_system(LoadOtherFrom,'Tag','PowerManagementSystem/LoadOthersFrom/PowerDesiredHighPriority');
PowerDesiredHighPriorityFrom = PowerDesiredHighPriorityFrom{1};
PowerDesiredHighPriorityFromName = get_param(PowerDesiredHighPriorityFrom,'Name');
set_param(PowerDesiredHighPriorityFrom,'NumInputs',int2str(nLoadOthers));

PowerOtherLoadsFrom = find_system(LoadOtherFromCentral,'Tag','FromLoads/MergerOtherLoads');
PowerOtherLoadsFrom = PowerOtherLoadsFrom{1};
PowerOtherLoadsFromName = get_param(PowerOtherLoadsFrom,'Name');
set_param(PowerOtherLoadsFrom,'NumInputs',int2str(nLoadOthers));

deleteAllBlocks('FromPowerDesiredLowPriority',LoadOtherFrom);
deleteAllBlocks('FromPowerLowPriority',LoadOtherFrom);
deleteAllBlocks('FromPowerDesiredHighPriority',LoadOtherFrom);
deleteAllBlocks('FromPowerHighPriority',LoadOtherFrom);
deleteAllBlocks('FromLoadsOtherPower',LoadOtherFromCentral);


for i = 1:nLoadOthers
    swbId = getSwbId(LoadOther{i});
    set_param(LoadOther{i},'SwbId',int2str(swbId));
    addFromBlock(LoadOtherFrom,sprintf('PowerDesiredLowPriority%d',swbId),'FromPowerDesiredLowPriority',100,50*swbId,sprintf('%s/%d',PowerDesiredLowPriorityFromName,swbId));
    addFromBlock(LoadOtherFrom,sprintf('PowerLowPriority%d',swbId),'FromPowerLowPriority',100,200+50*swbId,sprintf('%s/%d',PowerLowPriorityFromName,swbId));
    addFromBlock(LoadOtherFrom,sprintf('PowerDesiredHighPriority%d',swbId),'FromPowerDesiredHighPriority',100,400+50*swbId,sprintf('%s/%d',PowerDesiredHighPriorityFromName,swbId));
    addFromBlock(LoadOtherFrom,sprintf('PowerHighPriority%d',swbId),'FromPowerHighPriority',100,600+50*swbId,sprintf('%s/%d',PowerHighPriorityFromName,swbId));
    addFromBlock(LoadOtherFromCentral,sprintf('PowerLoadOthers%d',swbId),'FromLoadsOtherPower',100,500+50*swbId,sprintf('%s/%d',PowerOtherLoadsFromName,swbId));
   
end

%% Thrusters
Thruster = find_system(mdl,'LookUnderMasks','all','FollowLinks','on','Tag','Thruster');
nThruster = length(Thruster);

if (nThruster == 0)
    error('There must at least be one Thruster block in the diagram');
end

% Check that each thruster has unique id:
uniqueIdTest = cell(1,nThruster);
for t = 1:nThruster
    thruster = eval(get_param(Thruster{t},'thruster'));
    i = thruster.ID;
    if (i < 1 || i > nThruster)
        error('The id of thruster must be between 1 and the number of thrusters. %s has id %d',Thruster{t},i);
    end
    if (isempty(uniqueIdTest{i}))
        uniqueIdTest{i} = Thruster{t};
    else
        error('Thruster id %d is used multiple times. Both for: %s and %s',i,uniqueIdTest{i},Thruster{t});
    end
end
    

% Add goto
ThrusterFrom = find_system(PMS,'LookUnderMasks','all','Tag','PowerManagementSystem/ThrusterFrom'); ThrusterFrom = ThrusterFrom{1};
ThrusterFromMerger = find_system(ThrusterFrom,'Tag','FromThrusters/Merger'); ThrusterFromMerger = ThrusterFromMerger{1};
ThrusterFromMergerName = get_param(ThrusterFromMerger,'Name');
ThrusterFrom2 = find_system(mdl,'LookUnderMasks','all','FollowLinks','on','Tag','ThrusterAllocation/FromThrusters'); ThrusterFrom2 = ThrusterFrom2{1};
ThrusterFromMerger2 = find_system(ThrusterFrom2,'Tag','FromThrusters/Merger'); ThrusterFromMerger2 = ThrusterFromMerger2{1};
ThrusterFromMergerName2 = get_param(ThrusterFromMerger2,'Name');
ThrusterLoadFrom = find_system(mdl,'LookUnderMasks','all','Tag','FromLoads'); ThrusterLoadFrom = ThrusterLoadFrom{1};
ThrusterLoadFromMerger = find_system(ThrusterLoadFrom,'Tag','FromLoads/MergerThruster'); ThrusterLoadFromMerger = ThrusterLoadFromMerger{1};
ThrusterLoadFromMergerName = get_param(ThrusterLoadFromMerger,'Name');
ForceFrom = find_system(mdl,'LookUnderMasks','all','FollowLinks','on','Tag','ForceFrom'); ForceFrom = ForceFrom{1};
SurgeFromMerger = find_system(ForceFrom,'Tag','ForceFrom/SurgeMerger'); SurgeFromMerger = SurgeFromMerger{1};
SurgeFromMergerName = get_param(SurgeFromMerger,'Name');
SwayFromMerger = find_system(ForceFrom,'Tag','ForceFrom/SwayMerger'); SwayFromMerger = SwayFromMerger{1};
SwayFromMergerName = get_param(SwayFromMerger,'Name');
YawFromMerger = find_system(ForceFrom,'Tag','ForceFrom/YawMerger'); YawFromMerger = YawFromMerger{1};
YawFromMergerName = get_param(YawFromMerger,'Name');



set_param(ThrusterFromMerger,'NumInputs',int2str(nThruster));
set_param(ThrusterFromMerger2,'NumInputs',int2str(nThruster));
set_param(ThrusterLoadFromMerger,'NumInputs',int2str(nThruster));
set_param(SurgeFromMerger,'Inputs',int2str(nThruster));
set_param(SwayFromMerger,'Inputs',int2str(nThruster));
set_param(YawFromMerger,'Inputs',int2str(nThruster));

deleteAllBlocks('FromThruster',ThrusterFrom);
deleteAllBlocks('FromThruster',ThrusterFrom2);
deleteAllBlocks('FromThrusterPower',ThrusterLoadFrom);
deleteAllBlocks('ThrusterSurge',ForceFrom);
deleteAllBlocks('ThrusterSway',ForceFrom);
deleteAllBlocks('ThrusterYaw',ForceFrom);
for t = 1:nThruster
    swbId = getSwbId(Thruster{t});
    set_param(Thruster{t},'SwbId',int2str(swbId));
    
    thruster = eval(get_param(Thruster{t},'thruster'));
    i = thruster.ID;
    gotoTag = sprintf('Thruster%d',i);
    addFromBlock(ThrusterFrom,gotoTag,'FromThruster',100,100+50*i,sprintf('%s/%d',ThrusterFromMergerName,i));
    addFromBlock(ThrusterFrom2,gotoTag,'FromThruster',100,100+50*i,sprintf('%s/%d',ThrusterFromMergerName2,i));
    
    gotoTag = sprintf('ThrusterPower%d',i);
    addFromBlock(ThrusterLoadFrom,gotoTag,'FromThrusterPower',100,100+50*i,sprintf('%s/%d',ThrusterLoadFromMergerName,i));
    
    gotoTag = sprintf('ThrusterSurge%d',i);
    addFromBlock(ForceFrom,gotoTag,'ThrusterSurge',100,25+25*i,sprintf('%s/%d',SurgeFromMergerName,i));
    
    gotoTag = sprintf('ThrusterSway%d',i);
    addFromBlock(ForceFrom,gotoTag,'ThrusterSway',100,225+25*i,sprintf('%s/%d',SwayFromMergerName,i));
    
    gotoTag = sprintf('ThrusterYaw%d',i);
    addFromBlock(ForceFrom,gotoTag,'ThrusterYaw',100,425+25*i,sprintf('%s/%d',YawFromMergerName,i));
end

%% Update Emergency Battery
Battery = find_system(mdl,'LookUnderMasks','all','Tag','EmergencyBattery');
BatteryFromCentral = find_system(mdl,'LookUnderMasks','all','Tag','FromLoads'); BatteryFromCentral = BatteryFromCentral{1};
BatteryFromCentralName = get_param(BatteryFromCentral,'name');
nEmergencyBatteries = length(Battery);

deleteAllBlocks('FromEmergencyBatteriesPower',BatteryFromCentral);
PowerBatteryFrom = find_system(BatteryFromCentral,'Tag','FromLoads/MergerEmergencyBattery');
PowerBatteryFrom = PowerBatteryFrom{1};
PowerBatteryFromName = get_param(PowerBatteryFrom,'name');

BatteriesFrom = find_system(mdl,'IncludeCommented','on','Tag','PowerManagementSystem/FromBatteries');
BatteriesFrom = BatteriesFrom{1};
   
if (nEmergencyBatteries == 0)
    lines = get_param(PowerBatteryFrom,'LineHandles');
    if(lines.Outport > 0)
        delete_line(lines.Outport)
    end
    set_param(MergerLoadsCentral,'NumInports','2')
    set_param(BatteriesFrom,'Commented','on')
else
    if get_param(MergerLoadsCentral,'NumInports') ~= '3'
        set_param(MergerLoadsCentral,'NumInports','3')
        add_line(BatteryFromCentral,sprintf('%s/1',PowerBatteryFromName),sprintf('%s/3',MergerLoadsCentralName))
    end
    
    set_param(BatteriesFrom,'Commented','off')
    BatteriesFromMerger = find_system(BatteriesFrom,'Tag','PowerManagementSystem/FromBatteries/Merger');
    BatteriesFromMerger = BatteriesFromMerger{1};
    BatteriesFromMergerName = get_param(BatteriesFromMerger,'name');

    set_param(PowerBatteryFrom,'NumInputs',int2str(nEmergencyBatteries));
    set_param(BatteriesFromMerger,'NumInputs',int2str(nEmergencyBatteries));


    deleteAllBlocks('FromBattery',BatteriesFrom);
    
     for i = 1:nEmergencyBatteries
         swbId = getSwbId(Battery{i});
         set_param(Battery{i},'SwbId',int2str(swbId));
         addFromBlock(BatteriesFrom,sprintf('EmergencyBattery%d',swbId),'FromBattery',100,100+50*i,sprintf('%s/%d',BatteriesFromMergerName,i));
         addFromBlock(BatteryFromCentral,sprintf('PowerEmergencyBatteries%d',swbId),'FromEmergencyBatteriesPower',100,800+50*i,sprintf('%s/%d',PowerBatteryFromName,i));
 
     end
end


%% Gensets
generatorSetAC = find_system(mdl,'LookUnderMasks','all','FollowLinks','on','Tag','GensetAC');
nGeneratorSetAC = length(generatorSetAC);

generatorSetDC = find_system(mdl,'LookUnderMasks','all','FollowLinks','on','Tag','GensetDC');
nGeneratorSetDC = length(generatorSetDC);

generatorSet = [generatorSetAC; generatorSetDC];
nGeneratorSet = nGeneratorSetAC + nGeneratorSetDC;

if (nGeneratorSet == 0)
    error('There must at least be one Genset block in the diagram');
end

% Check that each thruster has unique id:
uniqueIdTest = cell(1,nGeneratorSet);
for t = 1:nGeneratorSet
    genset = eval(get_param(generatorSet{t},'genset'));
    i = genset.ID;
    if (i < 1 || i > nGeneratorSet)
        error('The id of generatorSet must be between 1 and the number of generatorSet. %s has id %d',generatorSet{t},i);
    end
    if (isempty(uniqueIdTest{i}))
        uniqueIdTest{i} = generatorSet{t};
    else
        error('The id of generatorSet must be between 1 and the number of generatorSet. ID = %d is used both for: %s and %s',i,uniqueIdTest{i},generatorSet{t});
    end
end

generatorSetFrom = find_system(PMS,'LookUnderMasks','all','Tag','PowerManagementSystem/FromGeneratorset'); generatorSetFrom = generatorSetFrom{1};
generatorSetFromMerger = find_system(generatorSetFrom,'Tag','PowerManagementSystem/FromGeneratorset/Merger'); generatorSetFromMerger = generatorSetFromMerger{1};
generatorSetFromMergerName = get_param(generatorSetFromMerger,'Name');
generatorSetElFrom = find_system(mdl,'Tag','FromGeneratorsetElectric'); generatorSetElFrom = generatorSetElFrom{1};
generatorSetElFromMerger = find_system(generatorSetElFrom,'Tag','FromGeneratorsetElectric/Merger'); generatorSetElFromMerger = generatorSetElFromMerger{1};
generatorSetElFromMergerName = get_param(generatorSetElFromMerger,'Name');

deleteAllBlocks('FromGeneratorset',generatorSetFrom);
deleteAllBlocks('FromGeneratorsetElectricFrom',generatorSetElFrom);
set_param(generatorSetFromMerger,'NumInputs',int2str(nGeneratorSet));
set_param(generatorSetElFromMerger,'NumInputs',int2str(nGeneratorSet));

for t = 1:nGeneratorSet
    swbId = getSwbId(generatorSet{t});
    set_param(generatorSet{t},'swbId',int2str(swbId));
    genset = eval(get_param(generatorSet{t},'genset'));
    i = genset.ID;
    gotoTag = sprintf('Generatorset%d',i);
    addFromBlock(generatorSetFrom,gotoTag,'FromGeneratorset',100,100+50*i,sprintf('%s/%d',generatorSetFromMergerName,i));
    
    gotoTag = sprintf('GeneratorsetElectric%d',i);
    addFromBlock(generatorSetElFrom,gotoTag,'FromGeneratorsetElectricFrom',100,100+50*i,sprintf('%s/%d',generatorSetElFromMergerName,i));
end

%% Breakers
Breaker = find_system(mdl,'LookUnderMasks','all','FollowLinks','on','Tag','SwitchboardBreaker');
nBreaker = length(Breaker);
BreakerFrom = find_system(mdl,'Tag','FromBreakers'); BreakerFrom = BreakerFrom{1};
BreakerFromMerger = find_system(BreakerFrom,'Tag','FromBreakers/MergerBreakers'); BreakerFromMerger = BreakerFromMerger{1};
BreakerFromMergerName = get_param(BreakerFromMerger,'Name');

set_param(BreakerFromMerger,'NumInputs',int2str(nBreaker));
deleteAllBlocks('BreakerFrom',BreakerFrom);
for i = 1:nBreaker
   set_param( Breaker{i},'id',int2str(i));
   gotoTag = sprintf('Breaker%d',i);
   addFromBlock(BreakerFrom,gotoTag,'BreakerFrom',100,100+50*i,sprintf('%s/%d',BreakerFromMergerName,i));

end

setColorScheme(mdl);
save_system(mdl);

end

function addFromBlock(parent,name,tag,x,y,destination)
blc = sprintf('%s/%s',parent,name);
add_block('built-in/From',blc,'Position', [x,y,x+200,y+20]);
set_param(blc,'Tag',tag);
set_param(blc,'GotoTag',name);
set_param(blc,'TagVisibility','global');
add_line(parent,sprintf('%s/1',name),destination);
end

function deleteAllBlocks(tag,parentBlock)
sys = find_system(parentBlock,'Tag',tag);
for i = 1:length(sys)
    lines = get_param(sys{i},'LineHandles');
    for j = 1:length(lines.Inport)
        if (lines.Inport(j) > 0)
            delete_line(lines.Inport(j));
        end
    end
    for j = 1:length(lines.Outport)
        if (lines.Outport(j) > 0)
            delete_line(lines.Outport(j));
        end
    end
    delete_block(sys{i});
end
end

function swbId = getSwbId(block)
    portHandles=get_param(block,'PortHandles');
    line = get_param( portHandles.Inport,'Line');
    SrcBlockHandle = get_param(line,'SrcBlockHandle');
    swbId = str2double(get_param(SrcBlockHandle,'swbId'));
    
end

