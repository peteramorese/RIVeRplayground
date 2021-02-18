function [] = plot_core(axs)

R = 2.391/2; % [m]

L = 5; % [m]
th = 0:pi/50:2*pi;

yunit = R*cos(th);
zunit = R*sin(th);

for i = 0:.02:L
    xunit = i*ones(length(yunit), 1);
    hh = plot3(axs, xunit, yunit, zunit, 'k');
    hh.Color(4) = 0.25;
end



end