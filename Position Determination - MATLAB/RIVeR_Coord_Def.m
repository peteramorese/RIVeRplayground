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
CORE.frame = [x_C, y_C, z_C];

clear x_C y_C z_C

%% Sensor Constants
SENSORS_true.constants.x_max = 315; % Camera X measurement maximum
SENSORS_true.constants.y_max = 207; % Camera Y measurement maximum
SENSORS_true.constants.gamma = 60 / 2; % [deg] Horizontal FOV half angle
SENSORS_true.constants.theta = 40 / 2; % [deg] Vertical FOV half angle

%% Sensor 1 Frame (defined in the Core Frame)
s1_C = [0.5; -0.55; -0.98]; % Position of Sensor 1 (defined in the C Frame)

SENSORS_true.S1.pos = s1_C; % Position of Sensor 1 (defined in the C Frame)
SENSORS_true.S1.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S1.frame = get_sensor_frame(s1_C, SENSORS_true.S1.point, CORE); % S1 Frame Matrix, represented in the C Frame
SENSORS_true.S1.Q_C = CORE.frame*SENSORS_true.S1.frame; % Direction Cosine Matrix: S1 Frame --> C Frame

clear s1_C

%% Sensor 2 Frame (defined in the Core Frame)
s2_C = [0.56; 0.52; -0.6]; % Position of Sensor 2 (defined in the C Frame)

SENSORS_true.S2.pos = s2_C; % Position of Sensor 2 (defined in the C Frame)
SENSORS_true.S2.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S2.frame = get_sensor_frame(s2_C, SENSORS_true.S2.point, CORE); % S2 Frame Matrix, represented in the C Frame
SENSORS_true.S2.Q_C = CORE.frame*SENSORS_true.S2.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

clear s2_C

%% Sensor 3 Frame (defined in the Core Frame)
s3_C = [0.56; 0.52; 0.6]; % Position of Sensor 3 (defined in the C Frame)

SENSORS_true.S3.pos = s3_C; % Position of Sensor 3 (defined in the C Frame)
SENSORS_true.S3.point = CORE.origin; % Reference point centered in the sensor's FOV
SENSORS_true.S3.frame = get_sensor_frame(s3_C, SENSORS_true.S3.point, CORE);  % S3 Frame Matrix, represented in the C Frame
SENSORS_true.S3.Q_C = CORE.frame*SENSORS_true.S3.frame; % Direction Cosine Matrix: S2 Frame --> C Frame

clear s3_C

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
CONSTANTS.m = 3; % Number of sensors
