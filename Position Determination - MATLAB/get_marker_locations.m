function [BAG_mod] = get_marker_locations(BAG, x, roll, pitch, yaw)
% get_marker_locations
%
%   Define 3D marker positions that are geometrically accurate for some
%   orientation, defined by the 3-2-1 Euler Angles roll, pitch, and yaw
%
%   Inputs:
%       BAG - Structure containing the bag dimensions and initial marker
%           positions
%       x - [3x1] [m] 3D Position of the center of the bag
%       roll - [deg] Roll angle for 3-2-1 Euler angles
%       pitch - [deg] Pitch angle for 3-2-1 Euler angles
%       yaw - [deg] Yaw angle for 3-2-1 Euler angles
%
%   Outputs:
%       MARKERS - Struct containing the 3D positions of each bag marker
%       after the rotation
%
%   Created By: Jordan Abell
%   Created On: 20 January 2021
%   Last Modified On: 20 January 2021

BAG_mod = BAG;

% Shift the marker positions
BAG_mod.x = x;
BAG_mod.markers.M1 = x + BAG.markers.M1;
BAG_mod.markers.M2 = x + BAG.markers.M2;
BAG_mod.markers.M3 = x + BAG.markers.M3;
BAG_mod.markers.M4 = x + BAG.markers.M4;
BAG_mod.markers.M5 = x + BAG.markers.M5;
BAG_mod.markers.M6 = x + BAG.markers.M6;

% Get the DCM corresponding to the rotation angles
Q = euler_to_DCM(roll, pitch, yaw);

% Apply the rotation to each marker positions
BAG_mod.markers.M1 = Q * BAG_mod.markers.M1;
BAG_mod.markers.M2 = Q * BAG_mod.markers.M2;
BAG_mod.markers.M3 = Q * BAG_mod.markers.M3;
BAG_mod.markers.M4 = Q * BAG_mod.markers.M4;
BAG_mod.markers.M5 = Q * BAG_mod.markers.M5;
BAG_mod.markers.M6 = Q * BAG_mod.markers.M6;

end

