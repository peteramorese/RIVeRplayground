function [Y_SID] = get_sim_data(SID, MID, t, R, CORE, BAG, SENSORS)
% get_sim_data
%
%   Create simulated pixy sensor data of specified markers for a given
%   sensor. Simulated data to be used in testing of the position Extended
%   Kalman Filter
%
%   Inputs:
%       SID - Sensor ID
%       MID - Marker IDs to simulate data for
%       t - [s] Time to simulate measurements over
%       R - Measurement Noise Covariance Matrix
%       CORE - Structure defining the Core Frame
%       BAG - Structure containing the true positions of each marker
%       SENSORS - Sensor positions and frames
%
%   Outputs:
%       Y_SID - Data package for SID as defined in the 'Data Definition'
%           page
%
%   Created By: Jordan Abell
%   Created On: 20 January 2021
%   Last Modified On: 20 January 2021

num_points = length(t); % Number of data points to simulate
num_mark = length(MID); % Number of markers to measure

Y_SID = cell(1, num_points); % Initialize variable

for k = 1:num_points
    
    y_k = [];
        
    for i = 1:num_mark
        
        MID_str = sprintf('M%d', MID(i)); % Marker ID to measure
        
        y_k_MID = get_yhat(BAG.markers.(MID_str), R, CORE, SENSORS, SID, 1);
        
        if ~isnan(y_k_MID(1))
            y_k(:, i) = [y_k_MID; MID(i); SID];
        end
        
    end
    
    Y_SID{k} = y_k;
    
end


end

