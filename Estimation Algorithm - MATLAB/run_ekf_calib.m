function [ekf, SENSORS_upd] = run_ekf_calib(t, Y, x0, R, BAG, sid, CORE, SENSORS, CONSTANTS)
% run_ekf_calib
%
%   Implementation of an Extended Kalman Filter (EKF) for the purpose of
%   estimating the positioning of a sensor to calibrate the measurement
%   system
%
%   Inputs:
%       t - [s] Time vector
%       Y - Cell array containing pixy measurements
%       x0 - [m] Initial estimate of the sid Sensor's position
%       R - Measurement Noise Covariance Matrix
%       BAG - Structure containing the positions of each marker
%       sid - Sensor ID being calibrated
%       SENSORS - Structure containing the sensor constants
%       CONSTANTS - Structure containing system-wide constants
%
%   Outputs:
%       ekf - Structure containing the results of the EKF
%       SENSORS_upd - Structure containing the updated position/pointing of the
%           calibrated sensor
%
%   Created By: Jordan Abell
%   Created On: 25 January 2021
%   Last Modified On: 25 January 2021

n = CONSTANTS.n; % Number of states

% Variable Initialization
ekf.x_hat = zeros(n, length(t)); % State estimate x_hat after the kth measurement update
ekf.P = zeros(n, n, length(t)); % Covariance matrix P after the kth measurement update
ekf.e_x = zeros(n, length(t)); % State Estimation Error
ekf.e_y = cell(1, length(t)); % Measurement Innovation
ekf.eps_x = zeros(1, length(t)-1); % NEES Statistic
ekf.eps_y = zeros(1, length(t)-1); % NIS Statistic
ekf.y = zeros(size(Y{2}, 2)*CONSTANTS.p, length(t)-1);
ekf.yhat = zeros(size(Y{2}, 2)*CONSTANTS.p, length(t)-1);

SENSORS_upd = SENSORS; % Copy SENSORS to update with new estimate
SENSORS_test = SENSORS;

% Initial Estimate & Covariance Matrix
ekf.x_hat(:, 1) = x0;
ekf.P(:, :, 1) = diag([1, 1, 1]);

for k = 1:length(t)-1
    
    yhat_kp1 = []; % Initialize array to store predicted measurement
    
    num_meas = size(Y{1, k+1}, 2); % Number of measurements
    
    for i = 1:num_meas % For each data packet (column) in Y{sid, k}
        
        mid = Y{1, k+1}(3, i); % Measured Marker ID
        MID_str = sprintf('M%d', mid);
        
        % Predicted Measurement
        yhat_kp1(length(yhat_kp1)+1:length(yhat_kp1)+2, 1) = get_yhat_cal(ekf.x_hat(:, k), mid, BAG, CORE, SENSORS, sid);

    end
    
    ekf.yhat(:, k+1) = yhat_kp1; % Predicted Measurement
    
    if ~isempty(Y{1, k+1})
        H_tilde = [];
        R_blk = [];
        
        for i = 1:num_meas
            MID_str = sprintf('M%d', Y{1, k+1}(3, i));
            
            H_tilde = [H_tilde; get_H_tilde_cal(ekf.x_hat(:, k), BAG.markers.(MID_str), SENSORS)];
            R_blk = blkdiag(R_blk, R);
        end
        
        y_kp1 = reshape(Y{1, k+1}(1:2, :), CONSTANTS.p*num_meas, 1); % Reshape the sensor measurements
        
        ekf.y(:, k+1) = y_kp1;
        
        ekf.e_y{k+1} = ekf.y(:, k+1) - ekf.yhat(:, k+1); % Measurement Innovation
        
        % Kalman Gain
        K_tilde = ekf.P(:, :, k) * H_tilde' * inv(H_tilde * ekf.P(:, :, k) * H_tilde' + R_blk);
        
        ekf.x_hat(:, k+1) = ekf.x_hat(:, k) + K_tilde * ekf.e_y{k+1};
        ekf.P(:, :, k+1) = (eye(CONSTANTS.n) - K_tilde*H_tilde)*ekf.P(:, :, k);
        
    else % If no measurement update
        
        ekf.x_hat(:, k+1) = ekf.x_hat(:, k);
        ekf.P(:, :, k+1) = ekf.P(:, :, k);
        
    end
    
    SID_str = sprintf('S%d', sid);
    ekf.e_x(:, k+1) = ekf.x_hat(:, k+1) - SENSORS.(SID_str).pos;
    
end

SENSORS_upd.(SID_str).pos = ekf.x_hat(:, end); % Update SENSORS with best new estimate
SENSORS_upd.(SID_str).frame = get_sensor_frame(SENSORS_upd.(SID_str).pos, SENSORS_upd.(SID_str).point, CORE); % Update
SENSORS_upd.(SID_str).Q_C = CORE.frame*SENSORS_upd.(SID_str).frame; % Update DCM

end

