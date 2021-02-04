function [SENSORS_pert] = perturb_sensors(SENSORS)
% perturb_sensors
%
%   Function to perturb the positions and pointing of each sensor to
%   simulate uncertainty in the placement and pointing of each pixy2
%
%   Inputs:
%       SENSORS - Structure containing the positions and frames of each
%           sensor
%
%   Outputs:
%       SENSORS_pert - Structure containing the perturbed positions and
%       frames of each sensor

SENSORS_pert = SENSORS; % Copy the structure

fields = fieldnames(SENSORS); % Get the names of each struct field

for i = 1:length(fields)
    
    if fields{i}(1) == 'S'
        
        fld_str = sprintf('%s', fields{i});
        
        SENSORS_pert.(fld_str).pos = SENSORS_pert.(fld_str).pos + 0.05*randn(3, 1);
        
    end
    
    
end

end

