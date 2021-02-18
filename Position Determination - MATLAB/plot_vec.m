function [] = plot_vec(origin, vec, clr, disp_name, plt_axes, plt_pnt)
%plot_vec
%   Plots a vector starting from 'origin' pointing to 'vec'
%   origin and vec must both be 3-dimensional vectors in the same
%   coordinate frame
%   The vector is plotted in the color 'clr' on axes 'plt_axes'

if plt_pnt == 1
    pnt_style = append('o', clr);
    plot3(plt_axes, origin(1) + vec(1), origin(2) + vec(2), origin(3) + vec(3), pnt_style, 'LineWidth', 2);
end

hh = quiver3(plt_axes, origin(1), origin(2), origin(3), 1.1*vec(1), 1.1*vec(2), 1.1*vec(3), clr, 'LineWidth', 2);
hh.Color(4) = 0.2;
text(origin(1) + 1.05*vec(1), origin(2) + 1.05*vec(2), origin(3) + 1.05*vec(3), disp_name, 'Color', clr);
end

