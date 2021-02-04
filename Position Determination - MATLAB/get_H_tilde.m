function [H_tilde] = get_H_tilde(x, CORE, SENSORS, SID)
% get_H_tilde
%
%   Returns the H_tilde matrix of the Sensor SID to be used in the EKF
%
%   Inputs:
%       CORE - Structure defining the Core Frame
%       SENSORS - Structure containing the sensor positions and frames
%       SID - Sensor ID
%
%   Outputs:
%       H_tilde - The nonlinear measurement function Jacobian
%
%   Created By: Jordan Abell
%   Created On: 20 January 2021
%   Last Modified On: 20 January 2021

% Sensor Constants
x_max = SENSORS.constants.x_max;
y_max = SENSORS.constants.y_max;
gamma = SENSORS.constants.gamma;
theta = SENSORS.constants.theta;

% Shorthand for computations
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

z = S(3, :)';

d1_a_dx = [-z(1)*(z(1)*(x(1) - s(1)) + z(2)*(x(2) - s(2)) + z(3)*(x(3) - s(3))),...
    -z(2)*(z(1)*(x(1) - s(1)) + z(2)*(x(2) - s(2)) + z(3)*(x(3) - s(3))),...
    -z(3)*(z(1)*(x(1) - s(1)) + z(2)*(x(2) - s(2)) + z(3)*(x(3) - s(3)))];

V_S1 = S'*C'*(x - s);
a = V_S1(3);

% H_tilde matrix from yhat = H*x + G
H_tilde = (1/a)*inv(DELTA'*DELTA) * DELTA' * inv(T) * S' * C' + ...
    inv(DELTA'*DELTA) * DELTA' * inv(T) * S' * C' * x * d1_a_dx;

end

