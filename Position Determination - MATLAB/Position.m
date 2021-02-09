%% ASEN 4028 - Senior Projects
%   Position Determination Implementation
%   Created By: Jordan Abell
%   Created On: 19 January 2021
%   Last Modified On: 19 January 2021

%% Initialization

% Housekeeping
clear; close all; clc;

% Coordinate Frame Definitions
RIVeR_Coord_Def;

% Constants
dT = 1/60; % [s] Sensor Data Rate
% dT = 0.1;

R = diag([2, 2]); % Measurement Noise Covariance R

rng(100);

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







