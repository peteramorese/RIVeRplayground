function [S] = get_sensor_frame(s, point, CORE)
% get_sensor_frame
%
%   Function to calculate the sensor's coordinate frame matrix based on the
%   position of the sensor
%
%   Inputs:
%       s - [m] Position vector of the sensor
%       point - [3x1] Reference point centered in the sensor's FOV
%       CORE - Structure defining the Core Frame
%
%   Outputs:
%       S - Matrix defining the coordinate frame of the sensor
%
%   Created By: Jordan Abell
%   Created On: 25 January 2021
%   Last Modified On: 25 January 2021

z_S1_C = point - s; % Vector defining the z_S1 axis, defined in the C Frame
z_S1_C = z_S1_C/norm(z_S1_C); % Normalize
x_S1_C = cross(z_S1_C, CORE.z); % Vector defining the x_S1 axis, defined in the C Frame
x_S1_C = x_S1_C/norm(x_S1_C); % Normalize
y_S1_C = cross(z_S1_C, x_S1_C); % Vector defining the y_S1 axis, defined in the C Frame

S = [x_S1_C, y_S1_C, z_S1_C]; % Coordinate frame matrix
end

