function [] = plot_markers(BAG)
% plot_markers
%
%   Plotting function used to visualize the bag markers in 3D space
%
%   Inputs:
%       BAG - Structure containint the positions of the markers
%
%   Outputs:
%
%   Created By: Jordan Abell
%   Created On: 20 January 2021
%   Last Modified On: 20 January 2021

figure;

for i = 1:6
    M_str = sprintf("M%d", i);
    
    x = BAG.markers.(M_str)(1);
    y = BAG.markers.(M_str)(2);
    z = BAG.markers.(M_str)(3);
    
    plot3(x, y, z, 'o', 'LineWidth', 2, 'DisplayName', M_str);
    hold on
    
end

grid on
legend('location', 'best')

