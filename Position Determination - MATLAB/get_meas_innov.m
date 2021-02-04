function [meas_innov] = get_meas_innov(t_vec, ekf)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
meas_innov = NaN*zeros(length(ekf.e_y{2}), length(t_vec));
for j = 1:length(t_vec)
    if length(ekf.e_y{j}) > 0
        meas_innov(:, j) = ekf.e_y{j};
    end
end
end

