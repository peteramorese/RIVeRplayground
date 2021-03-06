function [H_tilde_cal] = get_H_tilde_cal(s, x, SENSORS)
% get_H_tilde_cal
%
%   Function to find the H_tilde matrix for calibration of the sensor
%   positioning system
%
%   Inputs:
%       s - [m] Best estimate of the current sensor position
%       x - [m] Position of the marker used during calibration
%       SENSORS - Structure containing the sensor parameters
%
%   Outputs:
%       H_tilde_cal - H_tilde matrix used in calibration, equal to dh/ds
%
%   Created By: Jordan Abell
%   Created On: 24 January 2021
%   Last Modified On: 25 January 2021

% Sensor Constants
x_max = SENSORS.constants.x_max;
y_max = SENSORS.constants.y_max;
gamma = SENSORS.constants.gamma;
theta = SENSORS.constants.theta;

% Constant Matrices
T = ...
    [tand(gamma) 0 0;
    0 tand(theta) 0;
    0 0 1];
DELTA = ...
    [2/x_max 0;
    0 2/y_max;
    0 0];

%% Jacobians
% Reassign for easier typing
s1 = s(1);
s2 = s(2);
s3 = s(3);

x1 = x(1);
x2 = x(2);
x3 = x(3);

H_tilde_partial = zeros(3, 3); % Initialize

s_mag = sqrt(s1^2 + s2^2 + s3^2); % Magnitude of the vector s
s_denom = sqrt(s1^2/s_mag^2 + s2^2/s_mag^2); % Common denominator term in the Jacobian

H_tilde_partial(1, 1) = -(((-s2*x1/(s_mag*s_denom) + s1*x2/(s_mag*s_denom))*(s1/s_mag + s1^2*(x1-s1)/s_mag^3 - (x1-s1)/s_mag + ...
    s1*s2*(x2-s2)/s_mag^3 + s1*s3*(x3-s3)/s_mag^3))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + ...
    (s2*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)*x1/(2*s_mag*s_denom^3) + s1*s2*x1/(s_mag^3*s_denom) - ...
    s1*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)*x2/(2*s_mag*s_denom^3) - s1^2*x2/(s_mag^3*s_denom) + ...
    x2/(s_mag*s_denom))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(1, 2) = -(((-s2*x1/(s_mag*s_denom) + s1*x2/(s_mag*s_denom))*(s2/s_mag + s1*s2*(x1-s1)/(s_mag^3) + s2^2*(x2-s2)/s_mag^3 - ...
    (x2-s2)/s_mag + s2*s3*(x3-s3)/s_mag^3))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + ...
    (s2*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)*x1/(2*s_mag*s_denom^3) + s2^2*x1/(s_mag^3*s_denom) - ...
    x1/(s_mag*s_denom) - s1*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)*x2/(2*s_mag*s_denom^3) - ...
    s1*s2*x2/(s_mag^3*s_denom))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(1, 3) = -(((-s2*x1/(s_mag*s_denom) + s1*x2/(s_mag*s_denom))*(s3/s_mag + s1*s3*(x1-s1)/(s_mag^3) + s2*s3*(x2-s2)/s_mag^3 + ...
    s3^2*(x3-s3)/s_mag^3 - (x3-s3)/s_mag))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + ...
    (s2*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)*x1/(2*s_mag*s_denom^3) + s2*s3*x1/(s_mag^3*s_denom) - ...
    s1*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)*x2/(2*s_mag*s_denom^3) - ...
    s1*s3*x2/(s_mag^3*s_denom))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(2, 1) = -(((-s1^2*s3/(s_mag^2*s_denom) - s2^2*s3/(s_mag^2*s_denom) - s3*(-s1^2/(s_mag^2*s_denom) - ...
    s2^2/(s_mag^2*s_denom)) + s1*s3*x1/(s_mag^2*s_denom) + s2*s3*x2/(s_mag^2*s_denom) + (-s1^2/(s_mag^2*s_denom) - ...
    s2^2/(s_mag^2*s_denom))*x3)*(s1/s_mag + s1^2*(x1-s1)/s_mag^3 - (x1-s1)/s_mag + s1*s2*(x2-s2)/s_mag^3 + ... 
    s1*s3*(x3-s3)/s_mag^3))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + ...
    (s1^2*s3*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    s2^2*s3*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    2*s1^3*s3/(s_mag^4*s_denom) + 2*s1*s2^2*s3/(s_mag^4*s_denom) - 2*s1*s3/(s_mag^2*s_denom) - ...
    s3*(s1^2*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    s2^2*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)/(2*s_mag^2*s_denom^3) + 2*s1^3/(s_mag^4*s_denom) + ...
    2*s1*s2^2/(s_mag^4*s_denom) - 2*s1/(s_mag^2*s_denom)) - s1*s3*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + ...
    2*s1/s_mag^2)*x1/(2*s_mag^2*s_denom^3) - 2*s1^2*s3*x1/(s_mag^4*s_denom) + s3*x1/(s_mag^2*s_denom) - ...
    s2*s3*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)*x2/(2*s_mag^2*s_denom^3) - ...
    2*s1*s2*s3*x2/(s_mag^4*s_denom) + (s1^2*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    s2^2*(-2*s1^3/s_mag^4 - 2*s1*s2^2/s_mag^4 + 2*s1/s_mag^2)/(2*s_mag^2*s_denom^3) + 2*s1^3/(s_mag^4*s_denom) + ...
    2*s1*s2^2/(s_mag^4*s_denom) - 2*s1/(s_mag^2*s_denom))*x3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(2, 2) = -(((-s1^2*s3/(s_mag^2*s_denom) - s2^2*s3/(s_mag^2*s_denom) - s3*(-s1^2/(s_mag^2*s_denom) - ...
    s2^2/(s_mag^2*s_denom)) + s1*s3*x1/(s_mag^2*s_denom) + s2*s3*x2/(s_mag^2*s_denom) + (-s1^2/(s_mag^2*s_denom) - ...
    s2^2/(s_mag^2*s_denom))*x3)*(s2/s_mag + s1*s2*(x1-s1)/s_mag^3 + s2^2*(x2-s2)/s_mag^3 - (x2-s2)/s_mag + ... 
    s2*s3*(x3-s3)/s_mag^3))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + ...
    (s1^2*s3*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    s2^2*s3*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    2*s1^2*s2*s3/(s_mag^4*s_denom) + 2*s2^3*s3/(s_mag^4*s_denom) - 2*s2*s3/(s_mag^2*s_denom) - ...
    s3*(s1^2*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    s2^2*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)/(2*s_mag^2*s_denom^3) + 2*s1^2*s2/(s_mag^4*s_denom) + ...
    2*s2^3/(s_mag^4*s_denom) - 2*s2/(s_mag^2*s_denom)) - s1*s3*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + ...
    2*s2/s_mag^2)*x1/(2*s_mag^2*s_denom^3) - 2*s1*s2*s3*x1/(s_mag^4*s_denom) - ...
    s2*s3*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)*x2/(2*s_mag^2*s_denom^3) - ...
    2*s2^2*s3*x2/(s_mag^4*s_denom) + s3*x2/(s_mag^2*s_denom) + (s1^2*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)/(2*s_mag^2*s_denom^3) + ...
    s2^2*(-2*s1^2*s2/s_mag^4 - 2*s2^3/s_mag^4 + 2*s2/s_mag^2)/(2*s_mag^2*s_denom^3) + 2*s1^2*s2/(s_mag^4*s_denom) + ...
    2*s2^3/(s_mag^4*s_denom) - 2*s2/(s_mag^2*s_denom))*x3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(2, 3) = -(((-s1^2*s3/(s_mag^2*s_denom) - s2^2*s3/(s_mag^2*s_denom) - s3*(-s1^2/(s_mag^2*s_denom) - ...
    s2^2/(s_mag^2*s_denom)) + s1*s3*x1/(s_mag^2*s_denom) + s2*s3*x2/(s_mag^2*s_denom) + (-s1^2/(s_mag^2*s_denom) - ...
    s2^2/(s_mag^2*s_denom))*x3)*(s3/s_mag + s1*s3*(x1-s1)/s_mag^3 + s2*s3*(x2-s2)/s_mag^3 + s3^2*(x3-s3)/s_mag^3 - ...
    (x3-s3)/s_mag))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + ...
    (s1^2*s3*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)/(2*s_mag^2*s_denom^3) + ...
    s2^2*s3*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)/(2*s_mag^2*s_denom^3) + ...
    2*s1^2*s3^2/(s_mag^4*s_denom) + 2*s2^2*s3^2/(s_mag^4*s_denom) - ...
    s3*(s1^2*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)/(2*s_mag^2*s_denom^3) + ...
    s2^2*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)/(2*s_mag^2*s_denom^3) + 2*s1^2*s3/(s_mag^4*s_denom) + ...
    2*s2^2*s3/(s_mag^4*s_denom)) - s1*s3*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)*x1/(2*s_mag^2*s_denom^3) - ...
    2*s1*s3^2*x1/(s_mag^4*s_denom) + s1*x1/(s_mag^2*s_denom)- ...
    s2*s3*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)*x2/(2*s_mag^2*s_denom^3) - ...
    2*s2*s3^2*x2/(s_mag^4*s_denom) + s2*x2/(s_mag^2*s_denom) + (s1^2*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)/(2*s_mag^2*s_denom^3) + ...
    s2^2*(-2*s1^2*s3/s_mag^4 - 2*s2^2*s3/s_mag^4)/(2*s_mag^2*s_denom^3) + 2*s1^2*s3/(s_mag^4*s_denom) + ...
    2*s2^2*s3/(s_mag^4*s_denom))*x3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(3, 1) = -(((s1^2/s_mag + s2^2/s_mag + s3^2/s_mag - s1*x1/s_mag - s2*x2/s_mag - s3*x3/s_mag)*...
    (s1/s_mag + s1^2*(x1-s1)/s_mag^3 - (x1-s1)/s_mag + s1*s2*(x2-s2)/s_mag^3 + s1*s3*(x3-s3)/s_mag^3))/...
    (-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + (-s1^3/s_mag^3 - s1*s2^2/s_mag^3 - s1*s3^2/s_mag^3 + ...
    2*s1/s_mag + s1^2*x1/s_mag^3 - x1/s_mag + s1*s2*x2/s_mag^3 + s1*s3*x3/s_mag^3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(3, 2) = -(((s1^2/s_mag + s2^2/s_mag + s3^2/s_mag - s1*x1/s_mag - s2*x2/s_mag - s3*x3/s_mag)*...
    (s2/s_mag + s1*s2*(x1-s1)/s_mag^3 + s2^2*(x2-s2)/s_mag^3 - (x2-s2)/s_mag + s2*s3*(x3-s3)/s_mag^3))/...
    (-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + (-s1^2*s2/s_mag^3 - s2^3/s_mag^3 - s2*s3^2/s_mag^3 + ...
    2*s2/s_mag + s1*s2*x1/s_mag^3 + s2^2*x2/s_mag^3 - x2/s_mag + s2*s3*x3/s_mag^3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);
H_tilde_partial(3, 3) = -(((s1^2/s_mag + s2^2/s_mag + s3^2/s_mag - s1*x1/s_mag - s2*x2/s_mag - s3*x3/s_mag)*...
    (s3/s_mag + s1*s3*(x1-s1)/s_mag^3 + s2*s3*(x2-s2)/s_mag^3 + s3^2*(x3-s3)/s_mag^3 - (x3-s3)/s_mag))/...
    (-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag)^2) + (-s1^2*s3/s_mag^3 - s2^2*s3/s_mag^3 - s3^3/s_mag^3 + ...
    2*s3/s_mag + s1*s3*x1/s_mag^3 + s2*s3*x2/s_mag^3 + s3^2*x3/s_mag^3 - x3/s_mag)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);


%% H_tilde

H_tilde_cal = inv(DELTA'*DELTA)*DELTA'*inv(T)*H_tilde_partial; % dh/ds | shat

end

