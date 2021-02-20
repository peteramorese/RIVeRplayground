%% ASEN 4028 - Senior Projects
%   Position Determination Implementation
%   Created By: Jordan Abell
%   Created On: 19 January 2021
%   Last Modified On: 19 January 2021

%% Initialization

% Housekeeping
clear; clc; %close all; 

% Coordinate Frame Definitions
RIVeR_Coord_Def;

% Constants
dT = 1/60; % [s] Sensor Data Rate
% dT = 0.1;

R = diag([2, 2]); % Measurement Noise Covariance R

% rng(100);

clear t_start t_stop;

%% Calibration

% Calibration Times
t_start = 0; % [s] Initial Time
t_stop = 120; % [s] End Time
t = t_start:dT:t_stop; % [s] Time vector

MID_mat = [1; 2; 3]; % Matrix containing the Marker IDs corresponding to the Sensor IDs to measure them

SENSORS_calib = SENSORS_true; % Make a copy of the SENSORS structure, and calibrate it

Calibrate.markers.M1 = [0; 0.1; 0.1]; % Redefine the known position of M1
Calibrate.markers.M2 = [0.05; -0.05; 0.1]; % Redefine the known position of M2
Calibrate.markers.M3 = [0.1; -0.05; 0]; % Redefine the known position of M3

% filename = 'Y_sim.txt';

for sid = 1:3
    
    sid_str = sprintf('S%d', sid);
    
    fprintf('Calibrating Sensor %d...\n', sid);
    fprintf('\tPosition = [%.5f, %.5f, %.5f]\n', SENSORS_true.(sid_str).pos);

    Y_cal = get_sim_data(sid, MID_mat, t, R, CORE, Calibrate, SENSORS_true);
    
    filename = sprintf('Y_sim%d.txt', sid);
    
    write_sim_data(filename, Y_cal);

    x0 = SENSORS_true.(sid_str).pos + mvnrnd([0; 0; 0], 0.01*eye(3))';

    [ekf_cal, SENSORS_calib] = run_ekf_calib(t, Y_cal, x0, R, Calibrate, sid, CORE, SENSORS_calib, CONSTANTS);

    meas_innov = get_meas_innov(t, ekf_cal);
    innov_fig = figure; plot_meas_innov(t, meas_innov, 1, innov_fig);
    
    plot_estimate(t, ekf_cal, sprintf('Sensor S%d', sid), 0, 'calibration.png')

end


%% Simulate Data
close all; clc;

t_start = 0; % [s] Initial Time
t_stop = 120; % [s] End Time
t = t_start:dT:t_stop; % [s] Time vector

roll = 100*randn;
pitch = 100*randn;
yaw = 100*randn;

% fprintf('r = %.3f \t p = %.3f \t y = %.3f\n', roll, pitch, yaw);

% BAG1 = get_marker_locations(BAG, [0.21; 0.05; -0.15], roll, pitch, yaw);
% BAG1 = get_marker_locations(BAG, [2.55; 0.855; -0.75], roll, pitch, yaw);
BAG1.markers.M1 = [0; 0.1; 0.1]; % Redefine the known position of M1
BAG1.markers.M2 = [0.05; -0.05; 0.1]; % Redefine the known position of M2
BAG1.markers.M3 = [0.1; -0.05; 0]; % Redefine the known position of M3
BAG1.markers.M4 = [2.25; 0.85; -0.8];
BAG1.markers.M5 = [2.5; 0.9; -0.8];
BAG1.markers.M6 = [2.3; 0.75; -0.75];
BAG1.x = zeros(3, 1);

fig = figure;
vis_axes = axes;

plot_vec([0;0;0], CORE.x, 'k', 'Cx', vis_axes, 0);
hold on;
plot_vec([0;0;0], CORE.y, 'k', 'Cy', vis_axes, 0);
plot_vec([0;0;0], CORE.z, 'k', 'Cz', vis_axes, 0);

plot3(BAG1.x(1), BAG1.x(2), BAG1.x(3), 'ko', 'LineWidth', 2);

for i = 1:6
    m_str = sprintf('M%d', i);
    x = BAG1.markers.(m_str)(1);
    y = BAG1.markers.(m_str)(2);
    z = BAG1.markers.(m_str)(3);
    
    fprintf('%s \tx = %.3f\ty = %.3f\tz = %.3f\n', m_str, x, y, z)
    
    plot3(x,y,z, 'bo', 'LineWidth', 2);
    text(1.1*x, 1.1*y, 1.1*z, m_str);
end

for i = 1:CONSTANTS.m
    s_str = sprintf('S%d', i);
    x = SENSORS_true.(s_str).pos(1);
    y = SENSORS_true.(s_str).pos(2);
    z = SENSORS_true.(s_str).pos(3);
    
    plot3(x, y, z, 'ro', 'LineWidth', 2);
    text(1.1*x, 1.1*y, 1.1*z, s_str);
    
%     if i == 4 || i == 5
%         plot_FOV(SENSORS_true.(s_str).pos, SENSORS_true.(s_str).Q_C, SENSORS_true.constants.gamma, SENSORS_true.constants.theta, vis_axes);
%     end
%     plot_FOV(SENSORS_true.(s_str).pos, SENSORS_true.(s_str).Q_C, SENSORS_true.constants.gamma, SENSORS_true.constants.theta, vis_axes);
    
end

plot_core(vis_axes);
ylim([-3 3])

MID_mat = zeros(6, CONSTANTS.m);

for i = 1:CONSTANTS.m
    s_str = sprintf('S%d', i);
    for j = 1:6
        m_str = sprintf('M%d', j);
        m_vec = BAG1.x - BAG1.markers.(m_str);
        
        s_vec = BAG1.x - SENSORS_true.(s_str).pos;
        
        ang = acosd(dot(m_vec, s_vec)/(norm(m_vec)*norm(s_vec))); % [deg]
        
        yh = get_yhat(BAG1.markers.(m_str), R, CORE, SENSORS_true, i, 0);
        
%         fprintf('S%d   M%d   x = %.3f    y = %.3f    ang = %.3f [deg]\n', i, j, yh(1), yh(2), ang);
        
        if ~isnan(yh(1)) % && ang < 75
            
            MID_mat(j, i) = 1;
            
        end
        
    end
end

for sid = 1:CONSTANTS.m

    Y_cal = get_sim_data(sid, find(MID_mat(:, sid)), t, R, CORE, BAG1, SENSORS_true);
    
    filename = sprintf('Y_cal%d.txt', sid-1);
    
    write_sim_data(filename, Y_cal);
    
end

%%
S_positions = zeros(10, 3);
for i = 1:10
    s_str = sprintf('S%d', i);
    S_positions(i, :) = SENSORS_true.(s_str).pos;
end

S_positions = S_positions .* 39.3701;

fprintf('\t\t X [in] \t\t Y [in] \t\t Z [in]\n')
fprintf('----------------------------------------------------\n')

for i = 1:10
    s_str = sprintf('S%d', i);
    pos = SENSORS_true.(s_str).pos;
    fprintf('S%d\t\t %.4f \t\t %.4f \t\t %.4f\n', i, pos(1), pos(2), pos(3))
end


%% Estimation

% Measurement Times
t_start = 0; % [s] Initial Time
t_stop = 120; % [s] End Time
t = t_start:dT:t_stop; % [s] Time vector

MID_mat = [1, 1, 1;
    2, 2, 2]; % Matrix containing the Marker IDs corresponding to the Sensor IDs to measure them

m = CONSTANTS.m; % Number of sensors

Nsims = 1; % Number of simulations to run

NEES = zeros(Nsims, length(t)-1); % Initialize NEES results
NIS = zeros(Nsims, length(t)-1); % Initialize NIS results

for i = 1:Nsims
    
    fprintf('Simulation %d In Progress...\n', i);

    % Bag Rotations - 3-2-1 Euler Angles on [-180, 180]
    roll = -180 + (180 - (-180)).*rand(1); % [deg]
    pitch = -180 + (180 - (-180)).*rand(1); % [deg]
    yaw = -180 + (180 - (-180)).*rand(1); % [deg]

    x_cntr = 0.01*randn(3, 1); % [m] Random center position of the bag

    BAG_mod = get_marker_locations(BAG, x_cntr, roll, pitch, yaw);

    % SENSORS_pert = perturb_sensors(SENSORS);

    Y = cell(m, length(t)); % Initialize

    for sid = 1:CONSTANTS.m

        Y(sid, :) = get_sim_data(sid, MID_mat(:, sid), t, R, CORE, BAG_mod, SENSORS_true);

    end

    % Run the Extended Kalman Filter
    ekf = run_ekf(t, Y, 1, R, BAG_mod, CORE, SENSORS_calib, CONSTANTS);
    
    % Assign NEES and NIS statistics for each run
    NEES(i, :) = ekf.eps_x;
    NIS(i, :) = ekf.eps_y;

end

% Run the NEES and NIS tests
run_NEES_test(NEES, Nsims, CONSTANTS.n);
run_NIS_test(NIS, Nsims, CONSTANTS.p);

plot_estimate(t, ekf, "Marker M1", 0, 'EstimationError.png');







