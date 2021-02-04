function [yhat] = get_yhat_cal(s, mid, BAG, CORE, SENSORS, SID)
% get_yhat_cal
%
%   The nonlinear measurement function for yhat = H*x + G used to calibrate
%   the sensor system
%
%   Inputs:
%       s - [m] Estimate of the sensor position
%       BAG - Structure containing the true position of the measured
%           markers
%       CORE - Structure defining the Core Frame
%       SENSORS - Structure containing the position/pointing of each sensor
%       SID - Sensor ID being calibrate
%
%   Outputs:
%       yhat - [2x1] The SID Sensor's measurement of the marker M
%
%   Created By: Jordan Abell
%   Created On: 25 January 2021
%   Last Modified On: 25 January 2021

x_max = SENSORS.constants.x_max;
y_max = SENSORS.constants.y_max;
gamma = SENSORS.constants.gamma;
theta = SENSORS.constants.theta;

MID_str = sprintf('M%d', mid);
SID_str = sprintf('S%d', SID);
x = BAG.markers.(MID_str);

S = get_sensor_frame(s, SENSORS.(SID_str).point, CORE); % Matrix defining the sensor's pointing
C = CORE.frame;

T = ...
    [tand(gamma) 0 0;
    0 tand(theta) 0;
    0 0 1];
DELTA = ...
    [2/x_max 0;
    0 2/y_max;
    0 0];

ivec = [1; 1; -1];

V_S1 = S'*C'*(x - s);
a = V_S1(3);

H = (1/a)*inv(DELTA'*DELTA) * DELTA' * inv(T) * S' * C';
G = -(1/a)*inv(DELTA'*DELTA) * DELTA' * inv(T) * S' * C' * s + inv(DELTA'*DELTA) * DELTA' * ivec;

yhat = H*x + G; % Measurement Estimate

end

