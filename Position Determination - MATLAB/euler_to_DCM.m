function [Q] = euler_to_DCM(roll, pitch, yaw)
% euler_to_DCM
%
%   Converts 3-2-1 Euler Angle rotations to a Direction Cosine Matrix
%
%   Inputs:
%       roll - [deg] Roll angle
%       pitch - [deg] Pitch angle
%       yaw - [deg] Yaw angle
%
%   Outputs:
%       Q - Corresponding Direction Cosine Matrix
%
%   Created By: Jordan Abell
%   Created On: 20 January 2021
%   Last Modified On: 20 January 2021

a1 = deg2rad(roll);
a2 = deg2rad(pitch);
a3 = deg2rad(yaw);

Q = [cos(a2), sin(a2)*sin(a3), cos(a3)*sin(a2);
    sin(a1)*sin(a2), cos(a1)*cos(a3)-cos(a2)*sin(a1)*sin(a3), -cos(a1)*sin(a3)-cos(a2)*cos(a3)*sin(a1);
    -cos(a1)*sin(a2), cos(a3)*sin(a1)+cos(a1)*cos(a2)*sin(a3), cos(a1)*cos(a2)*cos(a3)-sin(a1)*sin(a3)];

end

