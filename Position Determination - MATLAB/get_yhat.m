function [yhat] = get_yhat(x, R, CORE, SENSORS, SID, round_flag)
% get_yhat
%
%   The nonlinear measurement function for yhat = H*x + G
%
%   Inputs:
%       x - [3x1] [m] 3D Position of the marker M, in the Core Frame
%       R - Measurement Noise Covariance Matrix
%       CORE - Structure defining the Core Frame
%       SENSORS - Structure containing sensor positions and coordinate
%           frames
%       SID - ID of the measuring sensor
%       round_flag - Flag to identify when to round measurements
%
%   Outputs:
%       yhat - [2x1] The SID Sensor's measurement of the marker M

x_max = SENSORS.constants.x_max;
y_max = SENSORS.constants.y_max;
gamma = SENSORS.constants.gamma;
theta = SENSORS.constants.theta;

SID_str = sprintf('S%d', SID);
S = SENSORS.(SID_str).frame;
s = SENSORS.(SID_str).pos;
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

yhat = H*x + G;

yhat = yhat + mvnrnd([0; 0], R)'; % Add measurement noise based on covariance R

if round_flag == 1
    yhat = round(yhat); % Due to discretization
end

% Account for angles and visibility
if yhat(1) > x_max || yhat(2) > y_max
    yhat = [];
end

end

