function R = q2R(v,theta)
% q2R (quaternion to rotation matrix)
%
% This is an embedded function in 'determineOrientation'

qr = cos(theta/2);
qi = sin(theta/2)*v(1);
qj = sin(theta/2)*v(2);
qk = sin(theta/2)*v(3);
s = 1/(qr^2 + qi^2 + qj^2 + qk^2);

R = [1-2*s*(qj^2+qk^2), 2*s*(qi*qj-qk*qr), 2*s*(qi*qk+qj*qr); ...
    2*s*(qi*qj+qk*qr), 1-2*s*(qi^2+qk^2), 2*s*(qj*qk-qi*qr); ...
    2*s*(qi*qk-qj*qr), 2*s*(qj*qk+qi*qr), 1-2*s*(qi^2+qj^2)];
end