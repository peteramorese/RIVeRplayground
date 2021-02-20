%% RIVeR Coord Def
%
%   Defines the coordinate frames

%% Core Frame
x_C = [1; 0; 0]; % X-Axis of the Core Frame
y_C = [0; 1; 0]; % Y-Axis of the Core Frame
z_C = [0; 0; 1]; % Z-Axis of the Core Frame

% Structure defining the Core Frame
CORE.x = x_C;
CORE.y = y_C;
CORE.z = z_C;
CORE.origin = [0; 0; 0];
CORE.drop = [2.2720; 0.8453; -0.8453];
CORE.frame = [x_C, y_C, z_C];

clear x_C y_C z_C

%% Sensor Constants
SENSORS_true.constants.x_max = 315; % Camera X measurement maximum
SENSORS_true.constants.y_max = 207; % Camera Y measurement maximum
SENSORS_true.constants.gamma = 60 / 2; % [deg] Horizontal FOV half angle
SENSORS_true.constants.theta = 40 / 2; % [deg] Vertical FOV half angle

%% Sensor 1 Frame (defined in the Core Frame)
% s1_C = [0.5; -0.55; -0.98] + [0.0888; 0.2078; 0.0223]; % Position of Sensor 1 (defined in the C Frame)
s1_C = [0.4142; 1.1214; 0.4052] + [0.0888; 0.2078; 0.0223];

SENSORS_true.S1.pos = s1_C; % Position of Sensor 1 (defined in the C Frame)
SENSORS_true.S1.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S1.frame = get_sensor_frame(s1_C, SENSORS_true.S1.point, CORE); % S1 Frame Matrix, represented in the C Frame
SENSORS_true.S1.Q_C = CORE.frame*SENSORS_true.S1.frame; % Direction Cosine Matrix: S1 Frame --> C Frame

clear s1_C

%% Sensor 2 Frame (defined in the Core Frame)
% s2_C = [0.56; 0.52; -0.6] + [-0.0370; 0.0325; 0.1514]; % Position of Sensor 2 (defined in the C Frame)
s2_C = [0.4142; -1.1214; 0.4052] + [-0.0370; 0.0325; 0.1514];

SENSORS_true.S2.pos = s2_C; % Position of Sensor 2 (defined in the C Frame)
SENSORS_true.S2.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S2.frame = get_sensor_frame(s2_C, SENSORS_true.S2.point, CORE); % S2 Frame Matrix, represented in the C Frame
SENSORS_true.S2.Q_C = CORE.frame*SENSORS_true.S2.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

clear s2_C

%% Sensor 3 Frame (defined in the Core Frame)
% s3_C = [0.56; 0.52; 0.6] + [-0.0235; 0.1107; -0.1603]; % Position of Sensor 3 (defined in the C Frame)
s3_C = [.75; 0.7242; 0.9479] + [-0.0235; 0.1107; -0.1603];

SENSORS_true.S3.pos = s3_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S3.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S3.frame = get_sensor_frame(s3_C, SENSORS_true.S3.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S3.Q_C = CORE.frame*SENSORS_true.S3.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

clear s3_C

%% Sensor 4 Frame (defined in the Core Frame)
s4_C = [.75; -0.7242; 0.9479] + [0.0538; 0.1834; -0.2259];

SENSORS_true.S4.pos = s4_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S4.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S4.frame = get_sensor_frame(s4_C, SENSORS_true.S4.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S4.Q_C = CORE.frame*SENSORS_true.S4.frame; % Direction Cosine Matrix: S2 Frame --> C Frame


%% Sensor 5 Frame (defined in the Core Frame)
s5_C = [1.000; 0; 1.1934] + [0.0862; 0.0319; -0.1308];

SENSORS_true.S5.pos = s5_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S5.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S5.frame = get_sensor_frame(s5_C, SENSORS_true.S5.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S5.Q_C = CORE.frame*SENSORS_true.S5.frame; % Direction Cosine Matrix: S2 Frame --> C Frame


%% Sensor 6 Frame (defined in the Core Frame)
s6_C = [1.772; 1.0353; 0.5978] + [-0.0434; 0.0343; 0.3578];

SENSORS_true.S6.pos = s6_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S6.point = CORE.drop; % Reference point centered in the sensor's FOV
SENSORS_true.S6.frame = get_sensor_frame(s6_C, SENSORS_true.S6.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S6.Q_C = CORE.frame*SENSORS_true.S6.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

%% Sensor 7 Frame (defined in the Core Frame)
s7_C = [2.772; 1.0353; 0.5978] + [0.2769; -0.1350; 0.3035];

SENSORS_true.S7.pos = s7_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S7.point = CORE.drop; % Reference point centered in the sensor's FOV
SENSORS_true.S7.frame = get_sensor_frame(s7_C, SENSORS_true.S7.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S7.Q_C = CORE.frame*SENSORS_true.S7.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

%% Sensor 8 Frame (defined in the Core Frame)
s8_C = [1.2720; 0; 1.1955] + [0.0725; -0.0063; 0.0715];

SENSORS_true.S8.pos = s8_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S8.point = CORE.drop; % Reference point centered in the sensor's FOV
SENSORS_true.S8.frame = get_sensor_frame(s8_C, SENSORS_true.S8.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S8.Q_C = CORE.frame*SENSORS_true.S8.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

%% Sensor 9 Frame (defined in the Core Frame)
s9_C = [3.272; 0; 1.1955] + [-0.0205; -0.0124; 0.1490];

SENSORS_true.S9.pos = s9_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S9.point = CORE.drop; % Reference point centered in the sensor's FOV
SENSORS_true.S9.frame = get_sensor_frame(s9_C, SENSORS_true.S9.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S9.Q_C = CORE.frame*SENSORS_true.S9.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

%% Sensor 10 Frame (defined in the Core Frame)
s10_C = [2.2720; -1.0353; 0.5978] + [0.1409; 0.1417; 0.0671];

SENSORS_true.S10.pos = s10_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S10.point = CORE.drop; % Reference point centered in the sensor's FOV
SENSORS_true.S10.frame = get_sensor_frame(s10_C, SENSORS_true.S10.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S10.Q_C = CORE.frame*SENSORS_true.S10.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

clear s4_C s5_C s6_C s7_C s8_C s9_C s10_C

%% Bag Definition
BAG.length = 0.41; % [m] Cargo bag length
BAG.width = 0.23; % [m] Cargo bag width
BAG.height = 0.24; % [m] Cargo bag height

BAG.x = [0; 0; 0]; % [m] Center of the bag
BAG.markers.M1 = [0; 0; 0.5*BAG.height]; % [m] Position of Marker M1 (Marker with the magnetic plate)
BAG.markers.M2 = [0; 0; -0.5*BAG.height]; % [m] Position of Marker M2
BAG.markers.M3 = [0.5*BAG.width; 0; 0]; % [m] Position of Marker M3
BAG.markers.M4 = [-0.5*BAG.width; 0; 0]; % [m] Position of Marker M4
BAG.markers.M5 = [0; 0.5*BAG.height; 0]; % [m] Position of Marker M5
BAG.markers.M6 = [0; -0.5*BAG.height; 0]; % [m] Position of Marker M6


%% Constants
CONSTANTS.n = 3; % Number of states
CONSTANTS.p = 2; % Number of sensor outputs
CONSTANTS.m = 10; % Number of sensors
