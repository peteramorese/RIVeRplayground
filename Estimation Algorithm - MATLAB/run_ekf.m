function [ekf] = run_ekf(t, Y, MID, R, BAG, CORE, SENSORS, CONSTANTS)
% run_ekf
%
%   Implementation of the Extended Kalman Filter for estimating the 3D
%   position of a stationary point
%
%   Inputs:
%       t - [s] Time vector
%       Y - Cell array containing pixy measurements
%       MID - Marker ID to estimate the position of
%       R - Measurement Noise Covariance Matrix
%       BAG - Structure containing the true positions of each marker
%       CORE - Structure definition the Core Frame
%       SENSORS - Structure containing the position/pointing of each sensor
%       CONSTANTS - Structure containing system-wide constants
%
%   Output:
%       ekf - Struct containing the results from the EKF
%
%   Created By: Jordan Abell
%   Created On: 18 January 2021
%   Last Modified On: 20 January 2021

n = CONSTANTS.n; % Number of states

% Variable Initialization
ekf.x_hat = zeros(n, length(t)); % State estimate x_hat after the kth measurement update
ekf.P = zeros(n, n, length(t)); % Covariance matrix P after the kth measurement update
ekf.e_x = zeros(n, length(t)); % State Estimation Error
ekf.e_y = cell(1, length(t)); % Measurement Innovation
ekf.eps_x = zeros(1, length(t)-1); % NEES Statistic
ekf.eps_y = zeros(1, length(t)-1); % NIS Statistic

% Initial Covariance Matrix
ekf.P(:, :, 1) = diag([5^2, 1.195^2, 1.195^2]);

for k = 1:length(t)-1 % For each time step
    
    vis_data_kp1 = []; % Initialize array to store data
    num_meas = 0; % Number of measurements of MID
    yhat_kp1 = []; % Initialize array to store predicted measurements
    
    for sid = 1:CONSTANTS.m % For each Sensor sid
        for i = 1:size(Y{sid, k}, 2) % For each data packet (column) in Y{sid, k}
            
            if Y{sid, k}(3, i) == MID % If MID is the marker being measured
                next_col = size(vis_data_kp1, 2)+1; % Next available column in vis_data
                vis_data_kp1(:, next_col) = Y{sid, k}(:, i); % Add the data packet to the end of vis_data
                num_meas = num_meas + 1;
                
                % Calculate the predicted sensor measurements without
                % measurement noise
                yhat_kp1(length(yhat_kp1)+1:length(yhat_kp1)+2, 1) = get_yhat(ekf.x_hat(:, k), zeros(2, 2), CORE, SENSORS, sid, 0);
            end
            
        end
    end
    
    if ~isempty(vis_data_kp1) % If there is a measurement update
        H_tilde = [];
        R_blk = [];
        
        for i = 1:num_meas
            H_tilde = [H_tilde; get_H_tilde(ekf.x_hat(:, k), CORE, SENSORS, vis_data_kp1(4, i))]; % NEED get_H_TILDE FUNCTION
            R_blk = blkdiag(R_blk, R);
        end
        
        y_kp1 = reshape(vis_data_kp1(1:2, :), CONSTANTS.p*num_meas, 1); % Reshape the sensor measurements
        
        ekf.e_y{k+1} = y_kp1 - yhat_kp1; % Measurement Innovation
        
        % NIS Statistic
        S_k = H_tilde*ekf.P(:, :, k)*H_tilde' + R_blk;
        ekf.eps_y(k) = ekf.e_y{k+1}'*inv(S_k)*ekf.e_y{k+1}./num_meas;
        
        % Kalman Gain
        K_tilde = ekf.P(:, :, k) * H_tilde' / (H_tilde * ekf.P(:, :, k) * H_tilde' + R_blk);
        
        ekf.x_hat(:, k+1) = ekf.x_hat(:, k) + K_tilde * ekf.e_y{k+1};
        ekf.P(:, :, k+1) = (eye(CONSTANTS.n) - K_tilde*H_tilde)*ekf.P(:, :, k);
        
        
    else % If no measurement update
        
        ekf.x_hat(:, k+1) = ekf.x_hat(:, k);
        ekf.P(:, :, k+1) = ekf.P(:, :, k);
        
    end

    MID_str = sprintf('M%d', MID);
    ekf.e_x(:, k+1) = ekf.x_hat(:, k+1) - BAG.markers.(MID_str);
    
    % NEES Statistic
    ekf.eps_x(k) = ekf.e_x(:, k+1)'*inv(ekf.P(:, :, k+1))*ekf.e_x(:, k+1);
    
end

end

